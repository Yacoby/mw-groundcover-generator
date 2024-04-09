cmake_minimum_required(VERSION 3.10)
project(MW_MESH_GEN)

set(Boost_USE_STATIC_LIBS ON)
set(wxWidgets_USE_STATIC ON)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include("${wxWidgets_USE_FILE}")

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(spdlog REQUIRED)

include_directories(${MW_MESH_GEN_SOURCE_DIR}/src)
include_directories(${MW_MESH_GEN_SOURCE_DIR}/GUI)
include_directories(${MW_MESH_GEN_SOURCE_DIR}/es)
include_directories(${MW_MESH_GEN_SOURCE_DIR}/esrw)
include_directories(${MW_MESH_GEN_SOURCE_DIR}/include)

FILE(GLOB es_sources ${MW_MESH_GEN_SOURCE_DIR}/es/*.cpp)
FILE(GLOB esrs_sources ${MW_MESH_GEN_SOURCE_DIR}/GUI/*.cpp)
FILE(GLOB sources ${MW_MESH_GEN_SOURCE_DIR}/src/*.cpp)
FILE(GLOB gui_sources ${MW_MESH_GEN_SOURCE_DIR}/GUI/*.cpp)

IF (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    add_executable(MWMeshGen WIN32 ${sources} ${gui_sources} ${es_sources} ${esrs_sources})
ELSE()
    add_executable(MWMeshGen ${sources} ${gui_sources} ${es_sources} ${esrs_sources})
ENDIF()

target_compile_features(MWMeshGen PRIVATE cxx_std_20)
target_link_libraries(MWMeshGen ${wxWidgets_LIBRARIES} spdlog::spdlog_header_only)

enable_testing()
add_subdirectory(tst)