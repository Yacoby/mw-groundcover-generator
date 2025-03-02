cmake_minimum_required(VERSION 3.10)
project(MW_GROUNDCOVER_GENERATOR VERSION 0.5.1)

set(Boost_USE_STATIC_LIBS ON)
set(wxWidgets_USE_STATIC ON)

# Remove CMAKE warnings: https://stackoverflow.com/questions/79146083/finding-boost-without-cmake-find-module-cmp0167
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(wxWidgets COMPONENTS core base REQUIRED)
include("${wxWidgets_USE_FILE}")

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(spdlog REQUIRED)

include_directories(${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/src)

FILE(GLOB sources
        ${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/src/logic/*.cpp
        ${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/src/esp/*.cpp)

FILE(GLOB gui_sources ${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/src/gui/*.cpp)
FILE(GLOB cli_sources ${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/src/cli/*.cpp)

IF (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    add_executable(MWGroundcoverGenerator WIN32 ${sources} ${gui_sources})
ELSEIF (APPLE)
    # I never got this to work correctly:
    # - The binary in the bundle seems to not have the executable bit set
    # - The bundle isn't signed? Maybe? (xattr -d com.apple.quarantine MWGroundcoverGenerator.app) allows it to work
    add_executable(MWGroundcoverGenerator MACOSX_BUNDLE ${sources} ${gui_sources})
    set_target_properties(MWGroundcoverGenerator PROPERTIES
            BUNDLE True
            MACOSX_BUNDLE_BUNDLE_NAME MWGroundcoverGenerator
            MACOSX_BUNDLE_BUNDLE_VERSION ${CMAKE_PROJECT_VERSION}
            MACOSX_BUNDLE_GUI_IDENTIFIER com.jacobessex.identifier.MWGroundcoverGenerator
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${CMAKE_PROJECT_VERSION}
    )
ELSE ()
    add_executable(MWGroundcoverGenerator ${sources} ${gui_sources})
ENDIF()

add_executable(MWGroundcoverGeneratorCLI ${sources} ${cli_sources})

target_compile_features(MWGroundcoverGenerator PRIVATE cxx_std_20)
target_compile_features(MWGroundcoverGeneratorCLI PRIVATE cxx_std_20)

target_link_libraries(MWGroundcoverGenerator ${wxWidgets_LIBRARIES} spdlog::spdlog_header_only)
target_link_libraries(MWGroundcoverGeneratorCLI spdlog::spdlog_header_only ${Boost_LIBRARIES})

enable_testing()
add_subdirectory(tst)