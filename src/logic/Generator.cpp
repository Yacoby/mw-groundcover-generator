#include "Generator.h"

#include <cassert>
#include <stdexcept>

#include <boost/random/uniform_real.hpp>
#include <boost/random/discrete_distribution.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include "xxhash.h"

#include "esp/ESFileContainer.h"
#include "esp/MutableEsp.h"

#include "Configuration.h"
#include "FixPosition.h"

template <typename T>
class Lazy {
private:
    std::function<T()> initializer;
    std::unique_ptr<T> instance;
public:
    Lazy(std::function<T()> initializer) : initializer(std::move(initializer)) {}

    T& get() {
        if (instance == nullptr) {
            instance = std::make_unique<T>(initializer());
        }
        return *instance;
    }
};

std::string toBase36(uint64_t value) {
    const char* digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    std::string result;

    if (value == 0) return "0";

    while (value > 0) {
        result = digits[value % 36] + result;
        value /= 36;
    }

    return result;
}

std::string toMeshId(std::map<std::string, std::string>& cache, const std::string& prefix, const std::string& value) {
    // This is not really for performance, but rather correctness. We can use this to ensure that we don't get a
    // collision
    if (cache.contains(value)) {
        return cache[value];
    }

    // 128 bit hash would be better, but there isn't a cross platform 128 bit type
    uint64_t hash = XXH3_64bits(value.data(), value.size());
    std::string hashBase36 = toBase36(hash);
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        if (it->second == hashBase36) {
            throw std::runtime_error("Found conflicting hash " + hashBase36 + " . One of the input was: " + value +
                                     " and the other was " + it->first);
        }
    }
    cache[value] = prefix + hashBase36;
    return cache[value];
}

struct ObjectInstance {
    const ObjectId objectId;
    const Vector3 position;
    const Vector3 rotation;
    const float scale;
};

void Generator::logCellStart(const GridId& grid, const ESFileRef& landRecordFile, const std::optional<std::string>& cellName, const std::optional<std::string>& cellRegion) {
    auto fileName = landRecordFile->getFilePath().filename().string();
    if (cellName.has_value() && cellRegion.has_value()) {
        logger->info(R"(Processing cell {}, cell name="{}, region="{}", using LAND record from="{}")", grid, cellName.value(), cellRegion.value(), fileName);
    } else if (cellName.has_value()) {
        logger->info(R"(Processing cell {}, cell name="{}", using LAND record from="{}")", grid, cellName.value(), fileName);
    } else if (cellRegion.has_value()) {
        logger->info(R"(Processing cell {}, region="{}", using LAND record from="{}")", grid, cellRegion.value(), fileName);
    } else {
        logger->info("Processing cell {}, using LAND record from=\"{}\"", grid, fileName);
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

void Generator::logConfiguration(const Configuration& configuration) {
    std::stringstream ss;
    ss << configuration;
    logger->info("Loaded configuration: {}", ss.str());
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

std::string getObjectId(std::map<std::string, std::string> meshIdCache, const std::string& objectPrefix, const ObjectPlacementPossibility& placement) {
    if (std::holds_alternative<ObjectId>(placement.idOrMesh)) {
        return std::get<ObjectId>(placement.idOrMesh).get();
    } else {
        auto& mesh = std::get<Mesh>(placement.idOrMesh);
        return toMeshId(meshIdCache, objectPrefix, mesh.get());
    }
}

const ObjectPlacementPossibility& Generator::getPlacement(const std::vector<ObjectPlacementPossibility>& placements,
                                                          const std::string& cat) {
    if (placements.empty()) {
        throw std::runtime_error(fmt::format(
                "Attempted to get mesh for placements, but no placements existed (category was '{}')", cat
        ));
    }

    std::vector<float> weights;
    for (const auto& placement: placements) {
        weights.push_back(placement.chance);
    }

    boost::random::discrete_distribution<> dist(weights.begin(), weights.end());

    return placements.at(dist(randomNumberSequence));
}

bool Generator::isIntersecting(float circleX, float circleY, float radius, float squareCenterX, float squareCenterY, float squareWidth) {
    float distX = abs(circleX - squareCenterX);
    float distY = abs(circleY - squareCenterY);

    if (distX > (squareWidth / 2 + radius)) return false;
    if (distY > (squareWidth / 2 + radius)) return false;

    if (distX <= (squareWidth / 2)) return true;
    if (distY <= (squareWidth / 2)) return true;

    double cornerDist = pow(distX - squareWidth / 2, 2) + pow(distY - squareWidth / 2, 2);

    return (cornerDist <= pow(radius, 2));
}

bool Generator::isPlacedNearExcludedTexture(
        ESFileContainer& fc,
        const PlaceMeshesBehaviour& placeBehaviour,
        float posx,
        float posy
) {
    // find every grid texture within x points of the placement
    for (const auto &exclusion: placeBehaviour.exclusions) {
        auto exclusionDistance = static_cast<float>(exclusion.distanceFromTexture.value_or(0));

        // find every ltex square that could intersect
        float minX = posx - exclusionDistance;
        float minY = posy - exclusionDistance;
        float maxX = posx + exclusionDistance;
        float maxY = posy + exclusionDistance;

        std::set<std::pair<int, int>> possiblyIntersectingLtexIndexes;
        for (float x = minX; x <= maxX; x += static_cast<float>(LTEX_GRID_SIZE)) {
            for (float y = minY; y <= maxY; y += static_cast<float>(LTEX_GRID_SIZE)) {
                possiblyIntersectingLtexIndexes.insert({
                    static_cast<int>(floor(x / LTEX_GRID_SIZE)),
                    static_cast<int>(floor(y / LTEX_GRID_SIZE)),
                });
            }
        }

        std::vector<std::pair<int, int>> intersectingLtexIndexes;
        for (const auto& ltexIdx: possiblyIntersectingLtexIndexes) {
            bool doesIntersectExactly = isIntersecting(
                    posx, posy, exclusionDistance,
                    static_cast<float>(ltexIdx.first) * LTEX_GRID_SIZE + static_cast<float>(LTEX_GRID_SIZE) / 2,
                    static_cast<float>(ltexIdx.second) * LTEX_GRID_SIZE + static_cast<float>(LTEX_GRID_SIZE) / 2,
                    LTEX_GRID_SIZE
            );
            if (doesIntersectExactly) {
                intersectingLtexIndexes.push_back(ltexIdx);
            }
        }

        for (const auto& ltexIdx: intersectingLtexIndexes) {
            auto ltex = fc.getLandTexture(
                    static_cast<float>(ltexIdx.first) * LTEX_GRID_SIZE + static_cast<float>(LTEX_GRID_SIZE) / 2,
                    static_cast<float>(ltexIdx.second) * LTEX_GRID_SIZE + static_cast<float>(LTEX_GRID_SIZE) / 2
            );
            if (ltex && (exclusion.texture == ltex->getPath() || exclusion.texture == ltex->getID())) {
                return true;
            }
        }
    }
    return false;
}

float Generator::getRandom(float min, float max) {
    // Using boost because uniform_real produces consistent output across platforms
    boost::uniform_real<float> dist(min, max);
    return dist(randomNumberSequence);
}

void Generator::generate() {
    try {
        MutableEsp esp;
        doGenerate(esp, [](const Configuration& cfg, ESFileContainer&, const MutableEsp&, const GridId&) { return true; });
    } catch (std::exception& e) {
        sendFailure(e);
    }
}

bool Generator::hasCellBeenChanged(const Configuration& cfg, ESFileContainer& fc, const MutableEsp& esp, const GridId& g, const RegenerateOptions& options) {
    auto file = fc.getLandFile(g.x, g.y);
    auto filename = file->getFilePath().filename().string();
    if (options.basePlugins.find(filename) != options.basePlugins.end()) {
        logger->info("Cell {} was last modified by {}, which is in the list of base plugins. Not regenerating", g, filename);
        return false;
    }
    logger->info("Cell {} was last modified by {} and is a candidate for regeneration", g, filename);

    const auto& cellIter = esp.exteriorCells.find(g);
    if (cellIter == esp.exteriorCells.end()) {
        if (options.regenerateWhenEmpty) {
            logger->info("Cell {} should be regenerated as there are no groundcover records in the plugin to patch and regenerate when empty is set to true", g);
            return true;
        } else {
            logger->info("Cell {} should be not be regenerated. Although it is empty, regenerate when empty is set to false", g);
            return false;
        }
    }

    bool hasFloatingGrass = false;
    if (options.regenerateIfFloatingGroundcover) {
        for (const auto& reference: cellIter->second->references) {
            auto expectedZ = fc.getHeightAt(reference.position.x, reference.position.y) + cfg.globalOffset;
            auto delta = fabs(expectedZ - reference.position.z);
            if (delta > PositionUpdater::MAX_ALLOWED_Z_ERROR) {
                logger->info("Found object instance {} with a height of {}, but expected it to have a height of around {}. The delta {} was greater than {}",
                             reference.name, reference.position.z, expectedZ, delta, PositionUpdater::MAX_ALLOWED_Z_ERROR);
                hasFloatingGrass = true;
                break;
            }
        }
    }

    if (options.regenerateIfFloatingGroundcover && !hasFloatingGrass) {
        logger->info("Should skip regeneration of {}, as no floating groundcover was detected", g);
        return false;
    }

    logger->info("Marking cell {} as needing regeneration", g);
    return true;
}

void Generator::generateFromExisting(const fs::path& existingPlugin, const RegenerateOptions& regenerateOptions) {
    try {
        MutableEsp esp;

        sendStatusUpdate(0, fmt::format("Loading plugin: {}", existingPlugin.filename().string()));
        try {
            esp.load(existingPlugin);
        } catch (std::exception& e) {
            std::throw_with_nested(std::runtime_error("Failed to load plugin " + existingPlugin.string()));
        }

        doGenerate(esp, [this, regenerateOptions] (const Configuration& cfg, ESFileContainer& fc, const MutableEsp& esp,const GridId& g) { return this->hasCellBeenChanged(cfg, fc, esp, g, regenerateOptions); });
    } catch (std::exception& e) {
        sendFailure(e);
    }
}

void Generator::doGenerate(MutableEsp& esp, const std::function<bool(const Configuration& cfg, ESFileContainer&,const MutableEsp&,const GridId&)>& cellUpdatePredicate) {
    if (mFiles.size() == 0) {
        throw std::runtime_error("No files to process");
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    sendStatusUpdate(0, "Loading configuration (ini) file");

    const auto configuration = loadConfigurationFromIni(logger, mIniLoc);
    logConfiguration(configuration);

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

    std::map<std::string, std::string> meshIdCache;
    std::map<GridId, std::vector<ObjectInstance>> placedInstances;

    const auto& cells = fc.getExteriorCellCoordinates();
    auto sortedCells = std::vector<GridId>(cells.begin(), cells.end());
    std::sort(sortedCells.begin(), sortedCells.end());

    int cellsProcessed = 0;
    for (const auto &cellCoord: sortedCells) {
        auto cx = cellCoord.x;
        auto cy = cellCoord.y;
        cellsProcessed++;
        int perCellPlacedMeshesCount = 0;
        int excludedDueToHeightBounds = 0;
        int excludedDueToTexture = 0;

        ESLandRef land = fc.getLand(cx, cy);
        assert(land);

        sendStatusUpdate(cellsProcessed / float(cells.size()) * 100, "Processing cell: " + std::to_string(cellCoord.x) + ", " + std::to_string(cellCoord.y));

        if (!land->getLandTextures().has_value()) {
            logger->info("Cell {} didn't have any textures", cellCoord);
            continue;
        }

        const auto& landTex = land->getLandTextures().value();

        ESFileContainer::CellInformation cellInformation = fc.getCellInformation(cx, cy);

        ESFileRef file = fc.getLandFile(cx, cy);
        logCellStart(cellCoord, file, cellInformation.name, cellInformation.region);

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

                // find the grass mesh if clumped
                auto& placements = placeBehaviour.placements;
                auto clumpedPlacement = Lazy<ObjectPlacementPossibility>([this, placements, deprecatedIniCat] { return getPlacement(placements, deprecatedIniCat); });

                //allow a max gap of more than 512
                if (placeBehaviour.gap > 512) {
                    float zz = 512.0f / placeBehaviour.gap;
                    float xx = getRandom(0, 100) / 100;
                    if (zz > xx) continue;
                }

                //put multiple grass objects down
                for (int gx = 0; gx < 512; gx += placeBehaviour.gap) {
                    for (int gy = 0; gy < 512; gy += placeBehaviour.gap) {

                        auto& placement = placeBehaviour.clump ? clumpedPlacement.get() : getPlacement(placements, deprecatedIniCat);

                        //calc the morrowind pos of the
                        float posx = tx * 512 + cx * 8192 + gx;
                        float posy = ty * 512 + cy * 8192 + gy;

                        //add a random element
                        auto posRand = placement.positionRandomization;
                        if (!(posRand.max == posRand.min && posRand.max == 0)) {
                            posx += getRandom(posRand.min, posRand.max);
                            posy += getRandom(posRand.min, posRand.max);
                        }

                        if (isPlacedNearExcludedTexture(fc, placeBehaviour, posx, posy)) {
                            excludedDueToTexture++;
                            continue;
                        }

                        Vector3 rot;
                        if (placement.alignToNormal) {
                            rot = fc.getAngleAt(posx, posy);
                        }
                        rot.z = getRandom(0, 2 * PI);

                        auto height = fc.getHeightAt(posx, posy);
                        float posZ = height + configuration.globalOffset;
                        if (posZ <= placement.heights.min || posZ >= placement.heights.max) {
                            excludedDueToHeightBounds++;
                            continue;
                        }

                        //get the scale of the object
                        float scale;
                        auto scaleRand = placement.scaleRandomization;
                        if (scaleRand.min == scaleRand.max) {
                            scale = scaleRand.max;
                        } else if (scaleRand.min < scaleRand.max) {
                            scale = getRandom(scaleRand.min, scaleRand.max);
                        } else {
                            throw std::runtime_error(
                                    "Invalid scale. Min (" + std::to_string(scaleRand.min) +
                                    ") was greater than the Max (" + std::to_string(scaleRand.max) + ")"
                            );
                        }

                        Vector3 pos;
                        pos.x = posx;
                        pos.y = posy;
                        pos.z = posZ;

                        // Might be a different cell
                        placedInstances[GridId::fromPosition(pos.x, pos.y)].push_back(
                                ObjectInstance{.objectId = ObjectId(getObjectId(meshIdCache, configuration.objectPrefix, placement)), .position = pos, .rotation = rot, .scale = scale}
                        );
                        perCellPlacedMeshesCount++;
                    }
                }

            }
        }

        logCellInformation(textureLogs);
        logger->info(
                "Placed {} objects in this and possibly surrounding cells. {} were excluded due to height bounds, {} because of banned textures",
                perCellPlacedMeshesCount, excludedDueToHeightBounds, excludedDueToTexture
        );

    }//	for

    esp.header.version = 1.3;
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

    sendStatusUpdate(100, "Writing STAT objects");
    logger->info("Writing STAT objects", mOut.string());

    {
        for (const auto &item: configuration) {
            if (!item.second.placeMeshesBehaviour.has_value()) {
                continue;
            }

            for (const auto& placement: item.second.placeMeshesBehaviour.value().placements) {
                if (!std::holds_alternative<Mesh>(placement.idOrMesh)) {
                    continue;
                }
                auto& mesh = std::get<Mesh>(placement.idOrMesh).get();
                auto id = toMeshId(meshIdCache, configuration.objectPrefix, mesh);
                if (!esp.statics.contains(id)) {
                    esp.addOrReplaceStatic(std::make_unique<Static>(Static(id, mesh)));
                }
            }
        }
    }

    sendStatusUpdate(0, "Constructing CELL records");
    logger->info("Constructing CELL records");

    int progress = 0;
    for (const auto &cellCoord: sortedCells) {
        sendStatusUpdate(static_cast<int>(progress++ * 100 / sortedCells.size()), "Constructing CELL records");

        const auto& cellRecordsItr = placedInstances.find(cellCoord);
        if (cellRecordsItr == placedInstances.end()) {
            continue;
        }

        if (!cellUpdatePredicate(configuration, fc, esp, cellCoord)) {
            logger->info("Skipped creating/updating cell {}", cellCoord);
            continue;
        }
        logger->info("Creating new cell record for {}", cellCoord);

        auto cell = std::make_unique<MutableCell>(MutableCell(cellCoord));


        ESFileContainer::CellInformation cellInformation = fc.getCellInformation(cellCoord.x, cellCoord.y);
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

    sendStatusUpdate(0, fmt::format("Saving plugin file to: {}", mOut.filename().string()));
    logger->info("Saving plugin file to: {}", mOut.string());
    esp.save(mOut);

    auto now = std::chrono::high_resolution_clock::now();
    long long seconds = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();

    logger->info("Generation complete in {} seconds", seconds);
    logger->flush();

    sendSuccess(fmt::format("Generation complete in {} seconds", seconds));
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
