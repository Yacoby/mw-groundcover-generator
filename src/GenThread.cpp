#include "GenThread.h"

#include <cassert>
#include <chrono>
#include <boost/random/uniform_real.hpp>
#include <boost/algorithm/string/join.hpp>

#include "Configuration.h"
#include "Funcs.h"
#include "ESBase.h"

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

std::vector<std::string> getNestedExceptionMessages(const std::exception& e) {
    std::vector<std::string> messages;
    messages.push_back(e.what());
    try {
        std::rethrow_if_nested(e);
    } catch(const std::exception& ne) {
        auto subMessages = getNestedExceptionMessages(ne);
        messages.insert(messages.end(), subMessages.begin(), subMessages.end());
    } catch(...) {

    }
    return messages;
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

void Generator::generate(
        std::shared_ptr<spdlog::logger> logger,
        std::function<void(int, std::string)> sendStatusUpdate,
        std::function<void(int duration)> sendSuccess,
        std::function<void(std::string)> sendFailure,
        const fs::path configurationLocation,
        std::vector<fs::path> inputFiles,
        const fs::path outputLocation,
        unsigned randomSeed) {
    try {
        Generator(logger, sendStatusUpdate, sendSuccess, sendFailure, configurationLocation, inputFiles, outputLocation,
                  randomSeed)
                .doGenerate();
    } catch (std::exception &e) {
        auto messages = getNestedExceptionMessages(e);
        sendFailure(boost::algorithm::join(messages, "\n"));
    }
}

void Generator::doGenerate() {
    if (mFiles.size() == 0) {
        sendFailure("No files to process");
        return;
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    gNumRecords = 0;
    gNumRecordPos = -1;


    sendStatusUpdate(0, "Loading configuration (ini) file");

    const auto configuration = loadConfigurationFromIni(mIniLoc);

    ESFileContainer fc = ESFileContainer();
    for (const auto& filePath: mFiles) {
        std::string fileName = filePath.filename().string();

        logger->info("Loading plugin: {}", filePath.string());
        sendStatusUpdate(0, "Loading: " + fileName);
        try {
            fc.loadDataFile(filePath);
        } catch (std::exception& e) {
            std::throw_with_nested(std::runtime_error("Failed to load plugin " + fileName));
        }
    }
    sendStatusUpdate(0, "Loaded all files");

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
                    float zz = 512 / (float) placeBehaviour.gap;
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
                                std::string t = exclusion.texture;
                                float r = exclusion.distanceFromTexture.value();

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

                                    auto ltex = fc.getLandTexture(tposx, tposy);
                                    if (ltex && (t == ltex->getPath() || t == ltex->getID())) {
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
        logger->info("Placed {} objects (although some may be in surrounding cells)", perCellPlacedMeshesCount);

    }//	for


    std::ofstream ofstream(mOut.string(),  std::ios::out | std::ios::binary);
    if (!ofstream.is_open()) {
        sendFailure("Failed to open output file: " + mOut.string());
        return;
    }

    sendStatusUpdate(100, "Writing STAT objects to output file");

    fileWriteEspHdr(ofstream);
    {
        for (const auto &item: configuration) {
            if (!item.second.placeMeshesBehaviour.has_value()) {
                continue;
            }

            for (const auto &item2: item.second.placeMeshesBehaviour.value().placements) {
                if (!std::holds_alternative<Mesh>(item2.idOrMesh)) {
                    continue;
                }
                auto mesh = std::get<Mesh>(item2.idOrMesh).get();
                fileWriteStatData(ofstream, "STAT", configuration.objectPrefix + item.first.toLegacyCategory() + std::to_string(item2.deprecatedId), mesh);
            }
        }
    }

    sendStatusUpdate(0, "Writing CELL records to the output file");

    int progress = 0;
    for (const auto &cellCoord: sortedCells) {
        int cx = cellCoord.first;
        int cy = cellCoord.second;

        sendStatusUpdate(progress++ * 100 / sortedCells.size(), "Writing CELL records to the output file");

        const auto& cellRecordsItr = placedInstances.find(GridId(cx, cy));
        if (cellRecordsItr == placedInstances.end()) {
            continue;
        }

        ESFileContainer::CellInformation cellInformation = fc.getCellInformation(cx, cy);

        Buff buffer;
        buffWriteCellStart(&buffer, 0, cx, cy, cellInformation.name.value_or(""));

        int frmr = 0;
        for (const auto &item: cellRecordsItr->second) {
            buffWriteObjData(&buffer, ++frmr, item.objectId.get(), item.scale, item.position.x, item.position.y,
                             item.position.z, item.rotation.x, item.rotation.y, item.rotation.z);
        }

        fileWriteCellHdr(&buffer, ofstream);
        fileWriteBuff(&buffer, ofstream);

        gNumRecords++;
    }

    //fix the number of records
    ofstream.seekp(gNumRecordPos);
    ofstream.write((char *) &gNumRecords, 4);

    ofstream.close();

    auto now = std::chrono::high_resolution_clock::now();
    long long seconds = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();

    logger->info("Generation complete in {} seconds", seconds);
    logger->flush();

    sendSuccess(seconds);
}

Generator::Generator(std::shared_ptr<spdlog::logger> logger,
                     const std::function<void(int, std::string)> sendStatusUpdateArg,
                     const std::function<void(int duration)> sendSuccessArg,
                     const std::function<void(std::string)> sendFailureArg,
                     const fs::path &configurationLocation,
                     const std::vector<fs::path> &files,
                     const fs::path &outputLocation,
                     unsigned randomSeed) : logger(logger),
                                            sendStatusUpdate(sendStatusUpdateArg),
                                            sendSuccess(sendSuccessArg),
                                            sendFailure(sendFailureArg),
                                            mOut(outputLocation),
                                            mIniLoc(configurationLocation),
                                            mFiles(files),
                                            randomNumberSequence(randomSeed) {
    logger->info("Creating generator with configuration:");
    logger->info("\tconfiguration location={}", configurationLocation.string());
    logger->info("\toutput location={}", outputLocation.string());
    logger->info("\tseed={}", randomSeed);
}
