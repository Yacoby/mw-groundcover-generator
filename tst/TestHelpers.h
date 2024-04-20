#ifndef MW_MESH_GEN_TESTHELPERS_H
#define MW_MESH_GEN_TESTHELPERS_H

#include <filesystem>
#include "spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> makeNullLogger();

void verifyEspsMatch(const std::filesystem::path& testPath, const std::filesystem::path& expectedPath);

#endif //MW_MESH_GEN_TESTHELPERS_H
