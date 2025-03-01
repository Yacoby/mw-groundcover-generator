#pragma once

#include <random>
#include <functional>
#include <filesystem>

#include <boost/random/mersenne_twister.hpp>
#include "spdlog/spdlog.h"

#include "esp/ESFile.h"
#include "esp/MutableEsp.h"
#include "esp/ESFileContainer.h"

#include "Configuration.h"

namespace fs = std::filesystem;

struct RegenerateOptions {
    bool regenerateIfFloatingGroundcover = true;
    bool regenerateWhenEmpty = true;
    std::set<std::string> basePlugins;
};

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
    std::function<void(const std::string&)> sendSuccess;
    std::function<void(const std::exception&)> sendFailure;

    fs::path mIniLoc;
    std::vector<fs::path> mFiles;
    fs::path mOut;

    boost::mt19937 randomNumberSequence;

    void logCellStart(const GridId& grid, const ESFileRef& landRecordFile, const std::optional<std::string>& cellName, const std::optional<std::string>& cellRegion);
    void logCellInformation(const std::vector<CellTextureLog>& cellTextureLogs);

    std::string getMesh(const std::vector<ObjectPlacementPossibility>& placements, const std::string &objectPrefix, const std::string &cat);
    bool isIntersecting(float circleX, float circleY, float radius, float squareCenterX, float squareCenterY, float squareWidth);
    bool isPlacedNearExcludedTexture(ESFileContainer& fc, const PlaceMeshesBehaviour& placeBehaviour, float posx, float posy);

    float getRandom(float min, float max);

    void logConfiguration(const Configuration&);
    bool hasCellBeenChanged(const Configuration& cfg, ESFileContainer&, const MutableEsp&, const GridId&, const RegenerateOptions&);
    void doGenerate(MutableEsp& esp, const std::function<bool(const Configuration& cfg, ESFileContainer&,const MutableEsp&,const GridId&)>& cellUpdatePredicate);
public:

    Generator(std::shared_ptr<spdlog::logger> logger,
              std::function<void(int, std::string)> sendStatusUpdate,
              std::function<void(const std::string& message)> sendSuccess,
              std::function<void(const std::exception&)> sendFailure,
              const fs::path &configurationLocation,
              const std::vector<fs::path> &inputFiles,
              const fs::path &outputLocation,
              unsigned randomSeed);

    void generate();
    void generateFromExisting(const fs::path& existingPlugin, const RegenerateOptions& regenerateOptions);
};
