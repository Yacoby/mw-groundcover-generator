This is a project mostly written in a hurry in 2009, with a few patches over the years. It is unsupported.

### MW Mesh Generator

### Introduction

This program automatically places meshes over the Morrowind landscape
with specific meshes being placed on specific ground textures.

The motivation for this was to enable grass mods in Morrowind

### Installation

- Download the binary from either:
  - [GitHub](https://github.com/Yacoby/mw-mesh-gen/releases/latest)
  - [Nexus Mods](https://www.nexusmods.com/morrowind/mods/23065)
- Extract the files to any location

### Using the software

#### General settings

- Settings Location - The configuration file with details of what grass to generate
- Output file - Where the generated file will be saved

#### Data files

This program supports multiple data files. You can load them
directly from your Morrowind.ini, OpenMW openmw.cfg or add them manually.

#### Configuration

The grass is generated according to a configuration file (an ini file). Included are some documented examples to explain
this format in the "docs" directory.

#### Troubleshooting

Logs are written to the file 'MWMeshGenLog.txt' and include information such as:
- The textures and configuration rules in each cell
- The plugins loaded

### Building from source

This requires [Boost](https://www.boost.org/), [wxWidgets](https://www.wxwidgets.org/) and [spdlog](https://github.com/gabime/spdlog). The UI is built using wxFormBuilder.

#### Mac

```shell
brew install boost wxwidgets spdlog
cmake .
make
```

### Thanks
Feedback and testing:
- Vality7
- PirateLord
- zackg

Grass configurations used for testing:
- [Aesthesia Groundcover](https://www.nexusmods.com/morrowind/mods/46377)
- [Remiros' Groundcover](https://nexusmods.com/morrowind/mods/46733)
