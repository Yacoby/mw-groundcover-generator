# Changelog

## 0.6.1 - Unreleased

# Added
- A setting for minimum allowed rotation from the vertical axis

## 0.6.0 - 2025-04-28

### Removed
- Removed the deprecated bSclRand and bPosRand configuration options

### Added
- Per placement properties for scale, rotation etc
- A setting for a maximum allowed rotation from the vertical axis

### Updated
- Generated STAT gids are now based on the mesh id to ensure no conflicts between mods

## 0.5.7 - 2025-03-20
### Fixed
- Fixed writing of unrelated record types (e.g. GMST) when using the Fix tool
- Fixed a crash caused by a configuration entry not having any placements. These configuration entries are now silently ignored when loading the configuration

## 0.5.6 - 2025-03-08
### Fixed
- Fixed error when loading .omwaddon that contain OpenMW specific record types

## 0.5.5 - 2025-03-03

### Added
- Support for setting the Morrowind location (rather than only inferring from the Windows registry)

### Fixed
- Improved error messages when loading corrupt/invalid mods

## 0.5.4 - 2025-03-02

### Fixed
- Removed usages of static offsets when using the Regenerate tool
- Various fixes for underwater groundcover
   - Support for LAND records which have no height or texture data
   - Correct default land height when there is no height data or LAND record

## 0.5.3 - 2024-08-02

### Fixed
- Removed use of static offsets when using the "Fix" tool to better support existing mods

## 0.5.2 - 2024-06-15

### Added
- Basic CLI (only supports generation) [[#1](https://github.com/Yacoby/mw-groundcover-generator/issues/1)]

### Fixed
- Bugfix to texture banning
- Support for existing grass mods that have MVRF subrecords

## 0.5.1 - 2024-05-01

### Added
- A guide on how to use the software

### Updated
- Updated how the "Regenerate" tool worked. It is now complex but should do a better job at regenerating the correct groundcover.

### Fixed
- Allowed exclusion offsets to be unset (previously an unset offset would crash the program)

## 0.5 - 2024-04-27

### Changed
- Banning placed objects from being near a texture now works correctly
- Change probabilities (iChanceX in the configuration) to be a weight. This means that they no longer need to sum to 100%

## 0.4 - 2024-04-24

### Added
- Tooling to fix existing floating groundcover
- Tooling to regenerate groundcover where land has changed
- UI fields are now saved and restored (for ease of testing)

### Fixed
- Placed objects are written to the correct CELL record in all cases

### Changed
- Program name from "MW Mesh Generator" to "Morrowind Groundcover Generator"
- ESM dependencies are added to generated files (for ease of testing)

### Removed
- Ubuntu build (as the Ubuntu GitHub runner has an old version of GCC which doesn't support modern language features)

## 0.3.3 - 2024-04-09

### Fixed
- Fixed coordinates in generated CELL records

## 0.3.2 - 2024-04-08

### Added
- Execution logs, which are written to `MWMeshGenLog.txt`

## 0.3.1 - 2024-04-08

### Fixed
- Correctly load textures when multiple files change the same land record
- Minor fixes to OpenMW resource resolution
- Performance

## 0.3 - 2024-04-06

### Changed
- Simplified configuration. Almost all configuration options now have sane defaults
- Deprecated `bPosRand` and `bSclRand`
- Removed z offset and id prefix from the UI and moved it to the config file
- No longer write `REGN`, `NAM5` to `CELL` records

### Removed
- Removed configuration options `sName`, `sRecType` and `sScript`
- Removed the Morrowind directory as a first class citizen. The option to set this is no longer present on the UI and
  instead the output path should set to the full path as it is no longer relative to the Morrowind directory path.

### Added
- Load order (i.e processing order) is now displayed in the UI
- First class OpenMW support
- Automated testing
  - Snapshot tests
  - Validation of existing mod configuration

### Fixed
- Crash (out of bounds memory access) when getting heights at a point
- Correctly sorts ESMs before ESPs
- Sort based on ESM dependencies as well as time


## 0.2 - 2024-03-29

### Added
- Added an option "bAlignObjectNormalToGround" (default true) which when set to false ("0") doesn't align the placed object to the angle of the terrain.
- Added error checking for common cases and support for displaying failures to users
- Allowed the dialog box to be resized to support differing font sizes etc
- Allow processing of cells further than 512 units from the origin
- Support for OS X (and probably Linux)
  - Ensuring all line endings are handled when reading ini files
  - Made path handling agnostic of separator
- Automated builds using github actions
- Use Steam and GOG registry keys to get the installation location

### Fixed
- Ini categories using the texture path and region (e.g. `[texuture.tga:region]`) now work correctly
- Use fixed size datatypes to allow building on a 64bit system (allowing greater RAM usage on 64bit systems)
- Improved performance by at least 30% by removing redundant lookups and copies
- Documentation to cover more details
- Various memory leaks have been addressed
- Simplified the code:
  - Load only required features from the datafiles which is intended to result in faster loading with less memory usage
  - Don't defer loading of LAND records. This simplification will increase memory usage when multiple data files are loaded
    (because we now load LAND records from all files, rather than just the latest file to touch the cell) but should result
    in improved performance and simpler code.

### Removed
- Any testing or support for 32 bit systems
- Windows XP is now unsupported due to the usage of [RegGetValueA](https://learn.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-reggetvaluea)
- Settings editor - as far as I know this wasn't used and as such isn't worth maintaining
