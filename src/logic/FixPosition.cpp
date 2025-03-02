#include "FixPosition.h"

PositionUpdater::PositionUpdater(std::shared_ptr<spdlog::logger> logger,
                                 std::function<void(int, std::string)> sendStatusUpdate,
                                 std::function<void(const std::string&)> sendSuccess,
                                 std::function<void(const std::exception&)> sendFailure,
                                 const std::vector<std::filesystem::path>& inputFiles,
                                 const std::filesystem::path& exitingPlugin, const std::filesystem::path& outputPath) : logger(logger), sendStatusUpdate(sendStatusUpdate),
                                                                                                                        sendSuccess(sendSuccess),
                                                                                                                        sendFailure(sendFailure), inputFiles(inputFiles),
                                                                                                                        exitingPlugin(exitingPlugin), outputPath(outputPath) {

}

void PositionUpdater::fix(bool operationIsDelete) {
    try {
        doFix(operationIsDelete);
    } catch (std::exception& e) {
        sendFailure(e);
    }
}

void PositionUpdater::doFix(bool operationIsDelete) {
    ESFileContainer fc = ESFileContainer();

    int fileProgress = 0;
    for (const auto& filePath: inputFiles) {
        std::string fileName = filePath.filename().string();

        logger->info("Loading plugin: {}", filePath.string());
        sendStatusUpdate(static_cast<int>(fileProgress++ / (float) inputFiles.size() * 100), "Loading: " + fileName);
        try {
            fc.loadDataFile(filePath);
        } catch (std::exception& e) {
            std::throw_with_nested(std::runtime_error("Failed to load plugin " + fileName));
        }
    }

    logger->info("Loading file to update: {}", exitingPlugin.filename().string());
    sendStatusUpdate(0, fmt::format("Loading file to update: {}", exitingPlugin.filename().string()));

    MutableEsp esp;
    try {
        esp.load(exitingPlugin);
    } catch (std::exception& e) {
        std::throw_with_nested(std::runtime_error("Failed to load plugin " + exitingPlugin.string()));
    }

    sendStatusUpdate(0, "Loaded all files");

    int offsetProcess = 0;
    std::map<std::string, std::map<int, int>> offsetCountsByObjId;
    for (const auto& cellPair: esp.exteriorCells) {
        auto& cell = cellPair.second;
        logger->info("Getting offsets from cell: {}, {}", cell->getGridPosition().x, cell->getGridPosition().y);
        sendStatusUpdate(static_cast<int>(offsetProcess++ / float(esp.exteriorCells.size()) * 100), "Getting offsets from cell: " + std::to_string(cell->getGridPosition().x) + ", " + std::to_string(cell->getGridPosition().y));

        for (auto& reference: cell->references) {
            auto landHeight = fc.getHeightAt(reference.position.x, reference.position.y);
            auto offset = static_cast<int>(round(reference.position.z - landHeight));
            offsetCountsByObjId[reference.name][offset]++;
        }
    }

    logger->info("Calculating per object offsets");
    sendStatusUpdate(0, "Calculating per object offsets");
    std::map<std::string, int> offsetsByObjId;
    for (const auto& offsetCountsForObj: offsetCountsByObjId) {
        auto maxCount = std::max_element(
                std::begin(offsetCountsForObj.second), std::end(offsetCountsForObj.second),
                [](const auto& p1, const auto& p2) {
                    return p1.second < p2.second;
                }
        );
        logger->info("\tOffset for {} is {}", offsetCountsForObj.first, maxCount->first);
        offsetsByObjId[offsetCountsForObj.first] = maxCount->first;
    }

    int cellUpdateProgress = 0;
    for (const auto& cellPair: esp.exteriorCells) {
        auto& cell = cellPair.second;
        logger->info("Processing cell: {}, {}", cell->getGridPosition().x, cell->getGridPosition().y);

        sendStatusUpdate(static_cast<int>(cellUpdateProgress++ / float(esp.exteriorCells.size()) * 100), "Processing cell: " + std::to_string(cell->getGridPosition().x) + ", " + std::to_string(cell->getGridPosition().y));

        long deleted = 0; // long to avoid casting the long from std::distance to int
        int updated = 0;

        if (operationIsDelete) {
            auto shouldDelete = [&](const CellReference& reference) {
                auto expectedZ = fc.getHeightAt(reference.position.x, reference.position.y) + offsetsByObjId[reference.name];
                return fabs(expectedZ - reference.position.z) > MAX_ALLOWED_Z_ERROR;
            };
            auto removedEndIter = std::remove_if(cell->references.begin(), cell->references.end(), shouldDelete);
            deleted += std::distance(removedEndIter, cell->references.end());;
            cell->references.erase(
                    removedEndIter,
                    cell->references.end()
            );
        } else {
            for (auto& reference : cell->references) {
                auto expectedZ = fc.getHeightAt(reference.position.x, reference.position.y) + offsetsByObjId[reference.name];
                if (fabs(expectedZ - reference.position.z) > MAX_ALLOWED_Z_ERROR) {
                    auto angle = fc.getAngleAt(reference.position.x, reference.position.y);
                    fc.getHeightAt(reference.position.x, reference.position.y);
                    reference.position.z = expectedZ;
                    reference.position.xrot = angle.x;
                    reference.position.yrot = angle.y;
                    updated++;
                }
            }
        }

        logger->info(
                "Found {} objects, Updated the position of {} and deleted {}",
                cell->references.size(), updated, deleted
        );
    }

    logger->info("Saving output file to {}", outputPath.string());
    sendStatusUpdate(0, fmt::format("Saving file to: {}", outputPath.filename().string()));

    try {
        esp.save(outputPath);
    } catch (std::exception& e) {
        std::throw_with_nested(std::runtime_error("Failed to save plugin " + exitingPlugin.string()));
    }
    logger->info("Save complete");

    logger->flush();
    sendSuccess(fmt::format("Update complete. Wrote output to {}", outputPath.filename().string()));
}
