#pragma once

#include <random>
#include <functional>
#include <filesystem>

#include <boost/random/mersenne_twister.hpp>
#include "spdlog/spdlog.h"

#include "Configuration.h"
#include "ESFile.h"

namespace fs = std::filesystem;

struct CellTextureLog {
    bool operator==(const CellTextureLog& rhs) const {
        return textureId == rhs.textureId &&
               textureName == rhs.textureName &&
               selector == rhs.selector;
    }

    bool operator!=(const CellTextureLog& rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const CellTextureLog& rhs) const {
        return std::tie(textureId, textureName, selector) < std::tie(rhs.textureId, rhs.textureName, rhs.selector);
    }

    bool operator>(const CellTextureLog& rhs) const {
        return rhs < *this;
    }

    bool operator<=(const CellTextureLog& rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const CellTextureLog& rhs) const {
        return !(*this < rhs);
    }

    std::string textureId;
    std::string textureName;
    std::optional<Selector> selector;
};

class Generator {
    std::shared_ptr<spdlog::logger> logger;

    std::function<void(int, const std::string&)> sendStatusUpdate;
    std::function<void(int duration)> sendSuccess;
    std::function<void(const std::string&)> sendFailure;

    fs::path mIniLoc;
    std::vector<fs::path> mFiles;
    fs::path mOut;

    boost::mt19937 randomNumberSequence;

    void logCellStart(int squX, int squY, const ESFileRef& landRecordFile, const std::optional<std::string>& cellName, const std::optional<std::string>& cellRegion);
    void logCellInformation(const std::vector<CellTextureLog>& cellTextureLogs);

    std::string getMesh(const std::vector<ObjectPlacementPossibility>& placements, const std::string &objectPrefix, const std::string &cat);

    float getRandom(float min, float max);

    Generator(std::shared_ptr<spdlog::logger> logger,
              std::function<void(int, std::string)> sendStatusUpdate,
              std::function<void(int duration)> sendSuccess,
              std::function<void(std::string)> sendFailure,
              const fs::path &configurationLocation,
              const std::vector<fs::path> &inputFiles,
              const fs::path &outputLocation,
              unsigned randomSeed);

    void doGenerate();
public:

    static void generate(
            std::shared_ptr<spdlog::logger> logger,
            std::function<void(int, std::string)> sendStatusUpdate,
            std::function<void(int duration)> sendSuccess,
            std::function<void(std::string)> sendFailure,
            const std::filesystem::path configurationLocation,
            const std::vector<fs::path> inputFiles,
            const std::filesystem::path outputLocation,
            unsigned randomSeed
    );
};
