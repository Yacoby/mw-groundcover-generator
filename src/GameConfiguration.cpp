#include "GameConfiguration.h"

#include <filesystem>
#include <vector>
#include <fstream>
#include <optional>
#include <ranges>
#include <set>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>

namespace fs = std::filesystem;

const std::set<std::string> SUPPORTED_EXTENSIONS = {
        ".esp", ".esm", ".owmaddon"
};

std::optional<std::string> getEnvString(const char* name) {
    auto value = getenv(name);
    if (value) {
        return value;
    }
    return std::nullopt;
}

std::filesystem::path getHomeDirectory() {
    auto homeEnv = getEnvString("HOME");
    if (homeEnv.has_value()) {
        return fs::path(homeEnv.value());
    } else {
        auto homeDriveEnv = getEnvString("HOMEDRIVE");
        auto homePathEnv = getEnvString("HOMEPATH");
        if (homeDriveEnv.has_value() && homePathEnv.has_value()) {
            return fs::path(homeDriveEnv.value()) / fs::path(homePathEnv.value());
        }
    }
    return fs::path();
}

fs::path getOpenMwCfgPath() {
    auto homeDirectory = getHomeDirectory();
    if (homeDirectory.empty()) {
        return fs::path();
    }

    // https://openmw.readthedocs.io/en/latest/reference/modding/paths.html
    std::vector<fs::path> searchPaths = {
            homeDirectory / ".config" / "openmw" / "openmw.cfg",
            homeDirectory / "Library" / "Preferences" / "openmw" / "openmw.cfg",
            homeDirectory / "Documents" / "My Games" / "OpenMW" / "openmw.cfg"
    };

    for (const auto &path: searchPaths) {
        if (fs::exists(path)) {
            return path;
        }
    }

    return fs::path();
}

OpenMwConfig loadOpenMwCfg(const std::filesystem::path& cfgPath) {
    std::vector<fs::path> dataPaths;
    std::vector<std::string> content;
    std::vector<std::string> groundCover;

    // boost::property_tree doesn't support duplicate values
    std::string line;
    std::ifstream ifs(cfgPath, std::ios::in);
    while (std::getline(ifs, line)) {
        boost::trim_if(line, [](char c) { return c == '\r' || c == '\n'; });

        if (line.find("content=") == 0) {
            content.push_back(
                    line.substr(std::string("content=").length())
            );
        } else if (line.find("data=") == 0) {
            auto value =  line.substr(std::string("data=").length());
            boost::trim_if(value, [](char c) { return c == '"'; });
            dataPaths.push_back(value);
        } else if (line.find("groundcover=") == 0) {
            groundCover.push_back(
                    line.substr(std::string("groundcover=").length())
            );
        }
    }

    return {
        .content = content,
        .groundCover = groundCover,
        .data = dataPaths,
    };
}

std::vector<std::filesystem::path> filterSupportedPlugins(const std::vector<std::filesystem::path>& plugins) {
    std::vector<std::filesystem::path> supportedPlugins;
    for (const auto& item: plugins) {
        auto extension = item.filename().extension().string();
        boost::algorithm::to_lower(extension);
        if (SUPPORTED_EXTENSIONS.find(extension) != SUPPORTED_EXTENSIONS.end()) {
            supportedPlugins.push_back(item);
        }
    }
    return supportedPlugins;
}

std::vector<std::filesystem::path> resolveOpenMwPluginPaths(const OpenMwConfig& cfg) {

    std::vector<std::string> plugins;
    plugins.insert(plugins.end(), cfg.content.begin(), cfg.content.end());
    plugins.insert(plugins.end(), cfg.groundCover.begin(), cfg.groundCover.end());

    std::vector<fs::path> paths;
    for (const auto &pluginName: plugins) {
        for (const auto &dataPath: std::ranges::reverse_view(cfg.data)) {
            if (fs::exists(dataPath / pluginName)) {
                paths.push_back(dataPath / pluginName);
                break;
            }
        }
    }
    return paths;
}

std::vector<std::string> loadMwIni(const std::filesystem::path& iniPath) {
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(iniPath.string(), pt);

    std::vector<std::string> plugins;
    for (int idx = 0;; ++idx) {
        auto iniKey = "Game Files.GameFile" + std::to_string(idx);
        auto file = pt.get_optional<std::string>(iniKey);
        if (!file.has_value()) {
            break;
        }
        plugins.push_back(file.value());
    }
    return plugins;
}