set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})

find_package(spdlog REQUIRED)

FILE(GLOB sources
        ${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/src/esp/*.cpp
        ${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/src/logic/*.cpp)

FILE(GLOB test_sources ${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/tst/*.cpp)

add_executable (MWGroundcoverGeneratorTests ${test_sources} ${sources})
target_compile_features(MWGroundcoverGeneratorTests PRIVATE cxx_std_20)
target_link_libraries (MWGroundcoverGeneratorTests spdlog::spdlog_header_only)

enable_testing()
add_test(NAME MWGroundcoverGeneratorTests COMMAND MWGroundcoverGeneratorTests WORKING_DIRECTORY ${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/tst)