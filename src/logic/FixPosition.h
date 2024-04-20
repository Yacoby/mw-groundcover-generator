#ifndef MW_MESH_GEN_FIXPOSITION_H
#define MW_MESH_GEN_FIXPOSITION_H

#include <string>
#include <functional>
#include <filesystem>

#include <boost/random/mersenne_twister.hpp>
#include "spdlog/spdlog.h"

#include "esp/ESFileContainer.h"
#include "esp/MutableEsp.h"

class PositionUpdater {
    std::shared_ptr<spdlog::logger> logger;

    std::function<void(int, const std::string&)> sendStatusUpdate;
    std::function<void(const std::string&)> sendSuccess;
    std::function<void(const std::exception&)> sendFailure;

    std::vector<std::filesystem::path> inputFiles;
    std::filesystem::path exitingPlugin;
    std::filesystem::path outputPath;

public:
    constexpr static const float DEFAULT_GENERATION_OFFSET = 10;
    constexpr static const float MAX_ALLOWED_Z_ERROR = 4;

    PositionUpdater(std::shared_ptr<spdlog::logger> logger,
                    std::function<void(int, std::string)> sendStatusUpdate,
                    std::function<void(const std::string&)> sendSuccess,
                    std::function<void(const std::exception&)> sendFailure,
                    const std::vector<std::filesystem::path>& inputFiles,
                    const std::filesystem::path& exitingPlugin,
                    const std::filesystem::path& outputPath);

    void fix(bool operationIsDelete = false);
private:
    void doFix(bool operationIsDelete);
};


#endif //MW_MESH_GEN_FIXPOSITION_H
