cmake_minimum_required (VERSION 2.8)
project (MW_MESH_GEN)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include( "${wxWidgets_USE_FILE}" )

find_package( Boost REQUIRED COMPONENTS system filesystem)
include_directories( ${Boost_INCLUDE_DIRS} )

include_directories (${MW_MESH_GEN_SOURCE_DIR}/src)
include_directories (${MW_MESH_GEN_SOURCE_DIR}/GUI)
include_directories (${MW_MESH_GEN_SOURCE_DIR}/es)
include_directories (${MW_MESH_GEN_SOURCE_DIR}/include)

FILE(GLOB es_sources ${MW_MESH_GEN_SOURCE_DIR}/es/*.cpp)

add_executable(MWMeshGen src/main.cpp src/Funcs.cpp GUI/GUI.cpp GUI/SettingsEditor.cpp GUI/GrassGen.cpp ${es_sources})

target_link_libraries(MWMeshGen ${wxWidgets_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
