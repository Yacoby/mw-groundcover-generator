
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

This requires [Boost](https://www.boost.org/), [wxWidgets](https://www.wxwidgets.org/), [spdlog](https://github.com/gabime/spdlog) and [fmt](https://github.com/fmtlib/fmt). The UI is built using wxFormBuilder.

### Mac

```shell
brew install boost wxwidgets spdlog fmt
cmake .
make
```

## Thanks
Feedback and testing:
- Vality7
- PirateLord
- zackg

Grass configurations used for testing:
- [Aesthesia Groundcover](https://www.nexusmods.com/morrowind/mods/46377)
- [Remiros' Groundcover](https://nexusmods.com/morrowind/mods/46733)
