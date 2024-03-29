# Changelog

## [Unreleased]

### Added
- Added an option "bAlignObjectNormalToGround" (default true) which when set to false ("0") doesn't align the placed object to the angle of the terrain.
- Added error checking for common cases and support for displaying failures to users
- Allowed the dialog box to be resized to support differing font sizes etc
- Allow processing of cells further than 512 units from the origin
- Support for OS X (and probably Linux)
  - Ensuring all line endings are handled when reading ini files
  - Made path handling agnostic of separator
- Automated builds using github actions

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
- Settings editor - as far as I know this wasn't used and as such isn't worth maintaining