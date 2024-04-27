
# Morrowind Groundcover Generator

## Introduction

This program automatically places objects over the Morrowind landscape
with objects meshes being placed on specific ground textures. 

The motivation for this tool was to enable groundcover mods in Morrowind

## Installation

- Download the binary from either:
  - [GitHub](https://github.com/Yacoby/mw-groundcover-generator/releases/latest)
  - [Nexus Mods](https://www.nexusmods.com/morrowind/mods/23065)
- Extract the files to any location

## Using the software

The program can be run from any directory

The left hand pane contains the mod list to use for generation and other operations (mods that don't modify land records
are ignored by the tools). These can be imported from Morrowind.ini, OpenMW or manually added.

The left hand pane contains the data files, these can be imported from Morrowind.ini, openmw.cfg or manually added.
Importing from a game configuration file will use Windows registry or
[OpenMW paths](https://openmw.readthedocs.io/en/stable/reference/modding/paths.html) to try and locate the correct file.

The right hand pane contains the tools:
- Generate - This is used to create new groundcover mods
- Regenerate - Given an existing mod, this attempts to detect when a cell has changed
- Fix - This updates floating groundcover, either by deleting it or moving it down to the ground

### Tool: Generate

This tool generates a new groundcover plugin. Use this if you want to generate a new mod

The follow has the following options:
- Configuration - Your configuration (ini) file containing the rules of where to place objects
- Output file - Where the new plugin will be written

#### Configuration

The grass is generated according to a configuration file (an ini file). Included are some documented examples to explain
this format in the "docs" directory.

### Tool: Regenerate

This tool updates an existing groundcover mod detecting any land changes in your load order and regenerating any changed cells

The follow has the following options:
- Configuration - The ini file from the groundcover mod you are using
- "Grouncover plugin to patch" - This is the groundcover plugin you want to update. You can set it from a file in the left hand pane by pressing "Set Target"
- Output file - Where the new plugin will be written

The detection of changed cells isn't foolproof. It only looks at land heights relative to existing groundcover and not the land texture. 
The heuristic used to detect if a cell should be regenerated is:
- For any object if `objects_z_position + 10 - existing_land_height` is outside 4 units of where it should be
- If the cell has no groundcover objects in it

This generates a copy of the plugin, and not a patch, so don't use the existing grass plugin, only the new one you have generated.

### Tool: Fix

This is a simple way to fix floating grass, either by repositioning it vertically or deleting it. This isn't a foolproof fix
as it doesn't take into account changed land textures, and won't generate groundcover for new land

The follow has the following options:
- "Grouncover plugin to patch" - This is the groundcover plugin you want to update. You can set it from a file in the left hand pane by pressing "Set Targeet"
- Output file - Where the new plugin will be written

This updates all groundcover where `objects_z_position + 10 - existing_land_height` is more than 4 units of where it should be.

This generates a copy of the plugin, and not a patch, so don't use the existing grass plugin, only the new one you have generated.

### Troubleshooting

Logs are written to the file 'MWMeshGenLog.txt' and include information such as:
- The textures and configuration rules in each cell
- The plugins loaded

## Building from source

This requires [Boost](https://www.boost.org/), [wxWidgets](https://www.wxwidgets.org/) and [spdlog](https://github.com/gabime/spdlog). The UI is built using wxFormBuilder.

### Mac

```shell
brew install boost wxwidgets spdlog
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
