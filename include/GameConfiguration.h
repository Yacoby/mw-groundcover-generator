#ifndef MW_MESH_GEN_GAMECONFIGURATION_H
#define MW_MESH_GEN_GAMECONFIGURATION_H

#include <filesystem>
#include <vector>

struct OpenMwConfig {
    std::vector<std::string> content;
    std::vector<std::string> groundCover;
    std::vector<std::filesystem::path> data;
};

std::filesystem::path getOpenMwCfgPath();

OpenMwConfig loadOpenMwCfg(const std::filesystem::path& cfg);

std::vector<std::filesystem::path> resolveOpenMwPluginPaths(const OpenMwConfig& cfg);

std::vector<std::string> loadMwIni(const std::filesystem::path& ini);

#endif //MW_MESH_GEN_GAMECONFIGURATION_H
