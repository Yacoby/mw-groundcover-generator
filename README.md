
# Morrowind Groundcover Generator  (aka MW Mesh Generator)


This program automatically places objects over the Morrowind landscape
with objects meshes being placed on specific ground textures. It also includes
functionality to update existing groundcover mods.

The motivation for this tool was to enable groundcover mods in Morrowind

## Installation

- Download the binary from either:
  - [GitHub](https://github.com/Yacoby/mw-groundcover-generator/releases/latest)
  - [Nexus Mods](https://www.nexusmods.com/morrowind/mods/23065)
- Extract the files to any location
- Run `MWGroundcoverGenerator`

## Using the software

See the User Guide (in the docs directory) for a user guide.

## Building from source

This uses [vcpkg](https://vcpkg.io/) for dependency management, vckpg is a submodule of this repository (so ensure the submodule is checked out prior to running the below commands)

To build:
```shell
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake ..
make
```

To run the tests:
```shell
make MWGroundcoverGeneratorTests
CTEST_OUTPUT_ON_FAILURE=1 make test
```

### UI

The UI is built using wxFormBuilder. The project file is `src/gui/Grass.fbp`

## Thanks
Feedback and testing:
- Vality7
- PirateLord
- zackg

Grass configurations used for testing:
- [Aesthesia Groundcover](https://www.nexusmods.com/morrowind/mods/46377)
- [Remiros' Groundcover](https://nexusmods.com/morrowind/mods/46733)
