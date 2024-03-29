This is a project mostly written in a hurry in 2009, with a few patches over the years. It is unsupported.

### MW Mesh Generator 0.2

### Introduction

This program automatically places meshes over the Morrowind landscape
with specific meshes being placed on specific ground textures.

The motivation for this was to enable grass mods in Morrowind

### Installation

- [Download the binary from Nexus Mods](https://www.nexusmods.com/morrowind/mods/23065)
- Extract the files to any location.

### Using the software

#### General settings

- Settings Location - The configuration file with details of what grass to generate
- Morrowind Location - This should be the location of your Morrowind folder (the folder with Morrowind.exe in)
- Output file - What the new esp will be called (saved to the "Data Files" folder)

- ID Append - The unique ID appended to the front of all created objects
- Z offset - Increase or decrease the height of all placed objects by a fixed amount

#### Data files

This program supports multiple data files. You can load them
directly from your Morrowind.ini or add files manually. The files you
add manually *must* exist in the "Data Files" folder your Morrowind directory.

The order the files show in this tool doesn't matter. As part of the generation, the files will be loaded in the same
order as Morrowind will load them.

#### Configuration

The grass is generated according to a configuration file (an ini file). Included are some documented examples to explain
this format

- grass\_cell\_ref.ini
- grass\_general\_info.ini

#### Known issues

- Setting `sRecType` to anything but `STAT` is unsupported

### Building from source

This requires [Boost](https://www.boost.org/) and [wxWidgets](https://www.wxwidgets.org/). The UI is built using wxFormBuilder.

#### Mac

```shell
brew install boost wxwidgets
cmake .
make
```

### Thanks

* Vality7 - Huge amount of feedback/testing
* PirateLord - Feedback/Ideas
* zackg - Poking me about bugs
