#pragma once

#include <random>
#include <functional>
#include <filesystem>

#include "Ini.h"

namespace fs = std::filesystem;

class Generator {
    std::function<void(int, const std::string&)> sendStatusUpdate;
    std::function<void()> sendSuccess;
    std::function<void(const std::string&)> sendFailure;

    fs::path mIniLoc;
    std::vector<fs::path> mFiles;
    fs::path mOut;

    std::string mIdBase;
    int mOffset;

    std::mt19937 randomNumberSequence;

    std::string getMesh(const std::list<GrassIni2::GrassMesh> &meshList, const std::string &cat);

    float getRandom(float min, float max);

    Generator(std::function<void(int, std::string)> sendStatusUpdate,
              std::function<void()> sendSuccess,
              std::function<void(std::string)> sendFailure,
              const fs::path &configurationLocation,
              const std::vector<fs::path> &inputFiles,
              const fs::path &outputLocation,
              const std::string &idPrefix,
              int offset,
              unsigned randomSeed);

    void doGenerate();
public:

    static void generate(
            std::function<void(int, std::string)> sendStatusUpdate,
            std::function<void()> sendSuccess,
            std::function<void(std::string)> sendFailure,
            const std::filesystem::path configurationLocation,
            const std::vector<fs::path> inputFiles,
            const std::filesystem::path outputLocation,
            const std::string idPrefix,
            int offset,
            unsigned randomSeed
    );
};
