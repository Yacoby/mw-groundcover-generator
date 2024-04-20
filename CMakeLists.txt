cmake_minimum_required(VERSION 3.10)
project(MW_GROUNDCOVER_GENERATOR)

set(Boost_USE_STATIC_LIBS ON)
set(wxWidgets_USE_STATIC ON)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include("${wxWidgets_USE_FILE}")

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(spdlog REQUIRED)

include_directories(${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/src)

FILE(GLOB sources
        ${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/src/logic/*.cpp
        ${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/src/esp/*.cpp
        ${MW_GROUNDCOVER_GENERATOR_SOURCE_DIR}/src/gui/*.cpp)

IF (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    add_executable(MWGroundcoverGenerator WIN32 ${sources})
ELSE()
    add_executable(MWGroundcoverGenerator ${sources})
ENDIF()

target_compile_features(MWGroundcoverGenerator PRIVATE cxx_std_20)
target_link_libraries(MWGroundcoverGenerator ${wxWidgets_LIBRARIES} spdlog::spdlog_header_only)

enable_testing()
add_subdirectory(tst)