#pragma once

#include <random>
#include <functional>
#include <boost/filesystem.hpp>

#include "Ini.h"

namespace fs = boost::filesystem;

class Generator {
    std::function<void(int, const std::string&)> sendStatusUpdate;
    std::function<void()> sendSuccess;
    std::function<void(const std::string&)> sendFailure;

    std::string mOut, mIdBase, mIniLoc;
    int mOffset;
    std::vector<std::string> mFiles;

    std::mt19937 randomNumberSequence;

    std::string getMesh(const std::list<GrassIni2::GrassMesh> &meshList, const std::string &cat);

    float getRandom(float min, float max);

    Generator(std::function<void(int, std::string)> sendStatusUpdate,
              std::function<void()> sendSuccess,
              std::function<void(std::string)> sendFailure,
              const std::string &out,
              const std::string &idBase,
              const std::string &iniLoc,
              std::vector<std::string> files,
              int offset);

    void doGenerate();
public:

    static void generate(
            std::function<void(int, std::string)> sendStatusUpdate,
            std::function<void()> sendSuccess,
            std::function<void(std::string)> sendFailure,
            const std::string &out,
            const std::string &idBase,
            const std::string &iniLoc,
            std::vector<std::string> files,
            int offset
    );
};
