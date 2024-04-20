#include "Generator.h"

#include <cassert>
#include <boost/random/uniform_real.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include "esp/ESFileContainer.h"
#include "esp/EspWriter.h"
#include "esp/MutableEsp.h"

#include "Configuration.h"
#include "FixPosition.h"

struct ObjectInstance {
    const ObjectId objectId;
    const Vector3 position;
    const Vector3 rotation;
    const float scale;
};

void Generator::logCellStart(int squX, int squY, const ESFileRef& landRecordFile, const std::optional<std::string>& cellName, const std::optional<std::string>& cellRegion) {
    auto fileName = landRecordFile->getFilePath().filename().string();
    if (cellName.has_value() && cellRegion.has_value()) {
        logger->info(R"(Processing cell {}, {}, cell name="{}, region="{}", using LAND record from="{}")", squX, squY, cellName.value(), cellRegion.value(), fileName);
    } else if (cellName.has_value()) {
        logger->info(R"(Processing cell {}, {}, cell name="{}", using LAND record from="{}")", squX, squY, cellName.value(), fileName);
    } else if (cellRegion.has_value()) {
        logger->info(R"(Processing cell {}, {}, region="{}", using LAND record from="{}")", squX, squY, cellRegion.value(), fileName);
    } else {
        logger->info("Processing cell {}, {}, using LAND record from=\"{}\"", squX, squY, fileName);
    }
}


void Generator::logCellInformation(const std::vector<CellTextureLog>& cellTextureLogs) {
    logger->info("Saw the following textures and applied rules:");
    std::set<CellTextureLog> seen;
    for (const auto& item: cellTextureLogs) {
        if (seen.find(item) != seen.end()) {
            continue;
        }
        seen.insert(item);

        std::string selectorStr = "none (no rule found)";
        if (item.selector.has_value()) {
            selectorStr = "\"" + item.selector.value().toLegacyCategory() + "\"";
        }
        logger->info("\ttexture id=\"{}\", (texture name=\"{}\"), applied configuration rule={}", item.textureId, item.textureName, selectorStr);
    }

    if (seen.empty()) {
        logger->info("\tnone");
    }
}

std::optional<Selector> getConfigurationSelector(
        const Configuration& configuration,
        const ESFileContainer::CellInformation& cellInfo,
        const std::string& texture,
        const std::string& textureId
) {
    std::vector textures({texture, textureId});
    for (const auto &tex: textures) {
        std::vector<Selector> selectors;
        if (cellInfo.name.has_value()) {
            selectors.emplace_back(tex, cellInfo.name.value());
            selectors.emplace_back(tex, "ANY_NAMED_CELL");
        }
        if (cellInfo.region.has_value() && !cellInfo.region->empty()) {
            selectors.emplace_back(tex, cellInfo.region.value());
        }
        selectors.emplace_back(tex);

        for (const auto &selector: selectors) {
            if (configuration.get(selector).has_value()) {
                return selector;
            }
        }
    }
    return std::nullopt;
}

std::string Generator::getMesh(const std::vector<ObjectPlacementPossibility>& placements, const std::string& objectPrefix, const std::string &cat) {
    std::string grassID = "UNKNOWN_GRASS";
    float meshRand = getRandom(0, 100);
    float meshChance = 1;
    for (const auto &item: placements) {
        meshChance += item.chance;
        if (meshChance > meshRand) {
            if (std::holds_alternative<ObjectId>(item.idOrMesh)) {
                grassID = std::get<ObjectId>(item.idOrMesh).get();
            } else {
                grassID = objectPrefix + cat + std::to_string(item.deprecatedId);
            }
            break;
        }
    }
    return grassID;
}

float Generator::getRandom(float min, float max) {
    // Using boost because uniform_real produces consistent output across platforms
    boost::uniform_real<float> dist(min, max);
    return dist(randomNumberSequence);
}

void Generator::generate() {
    try {
        MutableEsp esp;
        doGenerate(esp, [](const ESFileContainer&, GridId) { return true; });
    } catch (std::exception& e) {
        sendFailure(e);
    }
}

void Generator::generateFromExisting(const fs::path& existingPlugin) {
    try {
        MutableEsp esp;

        sendStatusUpdate(0, std::format("Loading plugin: {}", existingPlugin.filename().string()));
        try {
            esp.load(existingPlugin);
        } catch (std::exception& e) {
            std::throw_with_nested(std::runtime_error("Failed to load plugin " + existingPlugin.string()));
        }

        auto shouldUpdateCellPredicate = [&] (ESFileContainer& fc, GridId g) {
            const auto& cellIter = esp.exteriorCells.find(g);
            if (cellIter == esp.exteriorCells.end()) {
                return true;
            }
            const auto& cell = cellIter->second;

            for (const auto& reference: cell->references) {
                auto expectedZ = fc.getHeightAt(reference.position.x, reference.position.y) + PositionUpdater::DEFAULT_GENERATION_OFFSET;
                if (fabs(expectedZ - reference.position.z) > PositionUpdater::MAX_ALLOWED_Z_ERROR) {
                    return true;
                }

                // this doesn't check if the objects are on the correct texture
            }

            return cell->references.empty();
        };

        doGenerate(esp, shouldUpdateCellPredicate);
    } catch (std::exception& e) {
        sendFailure(e);
    }
}

void Generator::doGenerate(MutableEsp& esp, const std::function<bool(ESFileContainer&,GridId)>& cellUpdatePredicate) {
    if (mFiles.size() == 0) {
        throw std::runtime_error("No files to process");
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    sendStatusUpdate(0, "Loading configuration (ini) file");

    const auto configuration = loadConfigurationFromIni(mIniLoc);

    int pluginProgress = 0;
    ESFileContainer fc = ESFileContainer();
    for (const auto& filePath: mFiles) {
        std::string fileName = filePath.filename().string();

        logger->info("Loading plugin: {}", filePath.string());
        sendStatusUpdate(pluginProgress++ / (float)mFiles.size() * 100, "Loading: " + fileName);
        try {
            fc.loadDataFile(filePath);
        } catch (std::exception& e) {
            std::throw_with_nested(std::runtime_error("Failed to load plugin " + fileName));
        }
    }
    sendStatusUpdate(100, "Loaded all files");

    std::map<GridId, std::vector<ObjectInstance>> placedInstances;

    const auto& cells = fc.getExteriorCellCoordinates();
    auto sortedCells = std::vector<std::pair<int32_t, int32_t >>(cells.begin(), cells.end());
    std::sort(sortedCells.begin(), sortedCells.end());

    int cellsProcessed = 0;
    for (const auto &cellCoord: sortedCells) {
        auto cx = cellCoord.first;
        auto cy = cellCoord.second;
        cellsProcessed++;
        int perCellPlacedMeshesCount = 0;
        int excludedDueToHeightBounds = 0;
        int excludedDueToTexture = 0;

        ESLandRef land = fc.getLand(cx, cy);
        assert(land);

        sendStatusUpdate(cellsProcessed / float(cells.size()) * 100, "Processing cell: " + std::to_string(cx) + ", " + std::to_string(cy));


        ESFileRef file = fc.getLandFile(cx, cy);
        const auto& landTex = land->getLandTextures();

        ESFileContainer::CellInformation cellInformation = fc.getCellInformation(cx, cy);

        logCellStart(cx, cy, file, cellInformation.name, cellInformation.region);

        std::vector<CellTextureLog> textureLogs;
        textureLogs.reserve(16*16);

        //for each tex
        for (int tx = 0; tx < 16; tx++) {
            for (int ty = 0; ty < 16; ty++) {

                if (landTex[tx][ty] == 0) continue; //no tex I assume?

                if (!file->getLTexExists(landTex[tx][ty] - 1)) continue; //bad bad bad

                auto selector = getConfigurationSelector(configuration, cellInformation, file->getLTexPath(landTex[tx][ty] - 1), file->getLTex(landTex[tx][ty] - 1)->getID());
                textureLogs.emplace_back(CellTextureLog{
                    .textureId = file->getLTex(landTex[tx][ty] - 1)->getID(),
                    .textureName = file->getLTexPath(landTex[tx][ty] - 1),
                    .selector = selector,
                });

                if (!selector.has_value()) continue; //didn't manage to find a match
                const auto deprecatedIniCat = selector.value().toLegacyCategory();
                const auto& behaviour = configuration.get(selector.value()).value().get();

                if (!behaviour.placeMeshesBehaviour.has_value()) {
                    continue;
                }
                auto placeBehaviour = behaviour.placeMeshesBehaviour.value();


                //find the grass mesh
                std::string grassID;
                auto &meshList = placeBehaviour.placements;
                if (placeBehaviour.clump) {
                    grassID = getMesh(meshList, configuration.objectPrefix, deprecatedIniCat);
                }

                //allow a max gap of more than 512
                if (placeBehaviour.gap > 512) {
                    float zz = 512.0f / placeBehaviour.gap;
                    float xx = getRandom(0, 100) / 100;
                    if (zz > xx) continue;
                }

                //put multiple grass objects down
                for (int gx = 0; gx < 512; gx += placeBehaviour.gap) {
                    for (int gy = 0; gy < 512; gy += placeBehaviour.gap) {

                        if (!placeBehaviour.clump) {
                            grassID = getMesh(meshList, configuration.objectPrefix, deprecatedIniCat);
                        }

                        //calc the morrowind pos of the
                        float posx = tx * 512 + cx * 8192 + gx;
                        float posy = ty * 512 + cy * 8192 + gy;

                        //add a random element
                        auto posRand = placeBehaviour.positionRandomization;
                        if (!(posRand.max == posRand.min && posRand.max == 0)) {
                            posx += getRandom(posRand.min, posRand.max);
                            posy += getRandom(posRand.min, posRand.max);
                        }

                        //check if we really should place it, or if it is on a no place texture
                        {
                            bool doContinue = false;

                            for (const auto &exclusion: placeBehaviour.exclusions) {
                                auto r = static_cast<float>(exclusion.distanceFromTexture.value());
                                for (int p = 0; p < 4; p++) {

                                    float tposx = posx;
                                    float tposy = posy;

                                    switch (p) {
                                        case 0:
                                            tposx += r;
                                            break;
                                        case 1:
                                            tposx -= r;
                                            break;
                                        case 2:
                                            tposy += r;
                                            break;
                                        case 3:
                                            tposy -= r;
                                            break;
                                    }

                                    const auto& t = exclusion.texture;
                                    auto ltex = fc.getLandTexture(tposx, tposy);
                                    if (ltex && (t == ltex->getPath() || t == ltex->getID())) {
                                        excludedDueToTexture++;
                                        //ak, this is not good.
                                        doContinue = true;
                                        break;
                                    } //if (t==file2->getLTexPath(land...
                                    if (doContinue == true) break;
                                }//for ( int p = 0; p < 4; p++){
                                if (doContinue == true) break;
                            }//for ( int c = 0;ini.valueExists(iniCat, "sBan"+toString(c));++c ){
                            if (doContinue == true) continue;
                        }//{

                        auto height = fc.getHeightAt(posx, posy);
                        auto rotation = fc.getAngleAt(posx, posy);

                        float posZ = height + configuration.globalOffset;

                        Vector3 rot;
                        if (placeBehaviour.alignToNormal) {
                            rot = rotation;
                        }
                        rot.z = getRandom(0, 2 * PI);

                        if (posZ <= placeBehaviour.heights.min || posZ >= placeBehaviour.heights.max) {
                            excludedDueToHeightBounds++;
                            continue;
                        }

                        //get the scale of tthe object
                        float scale = 1;
                        auto scaleRand = placeBehaviour.scaleRandomization;
                        if (!(scaleRand.max == scaleRand.min && scaleRand.max == 1)) {
                            scale = getRandom(scaleRand.min, scaleRand.max);
                        }

                        Vector3 pos;
                        pos.x = posx;
                        pos.y = posy;
                        pos.z = posZ;

                        // Might be a different cell
                        placedInstances[GridId::fromPosition(pos.x, pos.y)].push_back(
                                ObjectInstance{.objectId = ObjectId(grassID), .position = pos, .rotation = rot, .scale = scale}
                        );
                        perCellPlacedMeshesCount++;
                    }
                }

            }
        }

        logCellInformation(textureLogs);
        logger->info("Placed {} objects in this and possibly surrounding cells. {} were excluded due to height bounds, {} because of banned textures", perCellPlacedMeshesCount, excludedDueToHeightBounds, excludedDueToTexture);

    }//	for

    esp.header.dependencies.clear();
    for (const auto& inputPath: mFiles) {
        std::string extension = inputPath.filename().extension().string();
        boost::algorithm::to_lower(extension);
        if (extension == ".esm") {
            esp.header.dependencies.push_back({
                      .name = inputPath.filename().string(),
                      .size = fs::file_size(inputPath),
            });
        }
    }

    sendStatusUpdate(100, "Writing STAT objects to output file");

    {
        for (const auto &item: configuration) {
            if (!item.second.placeMeshesBehaviour.has_value()) {
                continue;
            }

            for (const auto &item2: item.second.placeMeshesBehaviour.value().placements) {
                if (!std::holds_alternative<Mesh>(item2.idOrMesh)) {
                    continue;
                }
                auto id = configuration.objectPrefix + item.first.toLegacyCategory() + std::to_string(item2.deprecatedId);
                auto& mesh = std::get<Mesh>(item2.idOrMesh).get();
                esp.addOrReplaceStatic(std::make_unique<Static>(Static(id, mesh)));
            }
        }
    }

    sendStatusUpdate(0, "Constructing CELL records");

    int progress = 0;
    for (const auto &cellCoord: sortedCells) {
        int cx = cellCoord.first;
        int cy = cellCoord.second;

        sendStatusUpdate(static_cast<int>(progress++ * 100 / sortedCells.size()), "Constructing CELL records");

        if (!cellUpdatePredicate(fc, GridId(cx, cy))) {
            logger->info("Skipped creating/updating cell {}, {}", cx, cy);
            continue;
        }
        logger->info("Creating new cell record for {}, {}", cx, cy);

        auto cell = std::make_unique<MutableCell>(MutableCell(GridId(cx, cy)));

        const auto& cellRecordsItr = placedInstances.find(GridId(cx, cy));
        if (cellRecordsItr == placedInstances.end()) {
            continue;
        }

        ESFileContainer::CellInformation cellInformation = fc.getCellInformation(cx, cy);
        if (cellInformation.name.has_value()) {
            cell->setName(cellInformation.name.value());
        }

        for (const auto& item: cellRecordsItr->second) {
            auto& ref = cell->references.emplace_back();
            ref.name = item.objectId.get();
            ref.scale = item.scale;
            ref.position.x = item.position.x;
            ref.position.y = item.position.y;
            ref.position.z = item.position.z;
            ref.position.xrot = item.rotation.x;
            ref.position.yrot = item.rotation.y;
            ref.position.zrot = item.rotation.z;
        }

        esp.addOrReplaceCell(cell);
    }

    sendStatusUpdate(0, "Saving plugin file");
    esp.save(mOut);

    auto now = std::chrono::high_resolution_clock::now();
    long long seconds = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();

    logger->info("Generation complete in {} seconds", seconds);
    logger->flush();

    sendSuccess(std::format("Generation complete in {} seconds", seconds));
}

Generator::Generator(std::shared_ptr<spdlog::logger> logger,
                     const std::function<void(int, std::string)> sendStatusUpdate,
                     const std::function<void(const std::string& message)> sendSuccess,
                     const std::function<void(const std::exception&)> sendFailure,
                     const fs::path &configurationLocation,
                     const std::vector<fs::path> &files,
                     const fs::path &outputLocation,
                     unsigned randomSeed) : logger(logger),
                                            sendStatusUpdate(sendStatusUpdate),
                                            sendSuccess(sendSuccess),
                                            sendFailure(sendFailure),
                                            mOut(outputLocation),
                                            mIniLoc(configurationLocation),
                                            mFiles(files),
                                            randomNumberSequence(randomSeed) {
    logger->info("Creating generator with configuration:");
    logger->info("\tconfiguration location={}", configurationLocation.string());
    logger->info("\toutput location={}", outputLocation.string());
    logger->info("\tseed={}", randomSeed);
}
