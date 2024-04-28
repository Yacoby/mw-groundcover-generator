# Morrowind Groundcover Generator (aka MW Mesh Generator)

## Overview

![](img/overview.png)

1. The lefthand pane has the list of mods that the tools operate on
2. The righthand pane has a set of tools which operate on the plugins in the lefthand pane (1)

### Adding plugins
Plugins can be added from your Morrowind.ini (path detected from the system registry on Windows), openmw.cfg ([found using the common paths](https://openmw.readthedocs.io/en/stable/reference/modding/paths.html)) or manually add them by pressing "Add plugin file".

Plugins are displayed in load order. In most cases this uses the last modified time of the file. When importing from OpenMW this uses order defined in openmw.cfg.

When plugins have been imported from OpenMW, it is no longer possible to add plugins manually as it is not possible to determine the load order for the manually added plugin.

## Updating groundcover mods match your active mods

Most groundcover mods are created with a few plugins. Probably just Morrowind, Tribunal, Bloodmoon and maybe Tamriel Rebuilt. This program can update groundcover plugins where the grass differs  

### Quick fixes

Sometimes you have floating grass in your groundcover and you just want to remove it (or update the position)

![](img/fix.png)

1. Make sure your list of plugins in the left hand pane is correct (you probably want all your active plugins). Plugins that don't update the land will be automatically ignored
2. Select the groundcover plugin you want to update. You can
   * Select a mod on the left hand pane and press "Set target"
   * Press browse and manually select a mod file
3. Select the save location for the updated mod
4. Select what you want to do with floating groundcover (either update or delete)
5. Press "Create updated esp"

This will generate a copy of the plugin selected in step (2), but with floating grass repositioned.

The main caveat with this approach is that it doesn't account for changes in the texture on the land. e.g. using this tool with the [Trackless Grazelands](https://www.nexusmods.com/morrowind/mods/44194) mod you will still have in gaps in groundcover where the paths once were.

### More in depth fixes by regenerating some goundcover

If you want to regenerate the groundcover for cells that have changed, you can use the *Regenerate* tool

![](img/regenerate.png)

1. Make sure your list of plugins in the left hand pane is correct (you probably want all your active plugins). Plugins that don't update the land will be automatically ignored
2. Select the configuration (ini) file used to generate the groundcover plugin. These are often bundled with groundcover mods
3. Select the groundcover plugin you want to update. You can
    * Select a mod on the left hand pane and press "Set target"
    * Press browse and manually select a mod file
4. Select the save location for the updated mod
5. Press "Create updated esp"

This will generate a copy of the plugin selected in step (2), but with any cell which looks to have groundcover in the wrong position regenerated. 

For ease of use detection of changed landscape is based on heuristics rather than any exact mechanism. The heuristics are:
- Does the cell have any groundcover? If there is no groundcover the cell will be regenerated.
- Does any of the groundcover look out of place? If so the cell will be regenerated. (An object is out of place if `|expected_z_position - actual_z_position| > 4`)

## Making a new groundcover mod

This is a brief overview of creating a new groundcover mod. There is at least one other, slightly more practical guide on the [Project Tamriel website](https://www.project-tamriel.com/viewtopic.php?t=29)

![](img/generate.png)

1. Make sure your list of plugins in the left hand pane is correct. This should be just the plugins you want your groundcover mod to target.
2. Select the configuration (ini) file for your groundcover mod (see below)
3. Select the save location for the new groundcover mod
4. Press "Generate" to create the mod

### Configuration

The configuration uses the ini format.

#### Selectors

The ini sections (e.g. `[GL_Grass_03]`) represents a selector for a behaviour (more on that below). The selectors match
a ground texture, with optional additional parameters after a `:`.

- `[GL_Grass_03]` matches any area with the land texture `GL_Grass_03`
- `[GL_Grass_03:Balmora]` matches area with the land texture `GL_Grass_03` in any cell named Balmora
- `[GL_Grass_03:ANY_NAMED_CELL]` matches area with the land texture `GL_Grass_03` in any cell with a custom name (the `ANY_NAMED_CELL` suffix is fixed text which specifies this)
- `[GL_Grass_03:Bitter Coast]` matches area with the land texture `GL_Grass_03` in the region 'Bitter Coast'

Only one behaviour is picked (there is no merging of behaviours). The order in which selectors are checked is:
- Exact cell name (e.g. Balmora with `GL_Grass_03:Balmora`)
- Any cell with a name (e.g. `GL_Grass_03:ANY_NAMED_CELL`)
- The region (e.g. the Bitter Coast region with `GL_Grass_03:Bitter Coast`)
- The texture only (e.g. `GL_Grass_03`)

The motivation for these selectors was to allow for exclusion. For example the following configuration puts groundcover everywhere except in cells with a name (e.g. towns). It isn't known how useful this is in practice as I'm not aware of any groundcover mods using it.

```ini
[GL_Grass_03:ANY_NAMED_CELL]
bPlaceGrass=false

[GL_Grass_03]
bPlaceGrass=true
```


### Behaviours

Generates grass clumps
```ini
bRandClump=false ; Optional, default: false
```

Restrict the height at which meshes can be placed
```ini
fMinHeight=0 ; Optional, default: negative infinity
fMaxHeight=256 ; Optional, default: positive infinity
```

Applies a random position modifier to the groundcover
```ini
fPosMax=100 ; Optional, default: 0
fPosMin=-100 ; Optional, default: 0
```

Assigns a random scale to the object (note that the game engine caps the scale within some bounds)
```ini
fSclMax=1.5 ; Optional, default: 1
fSclMin=1 ; Optional, default: 1
```

When set to true, aligns objects normal to the ground (good for groundcover, not for trees)
```ini
bAlignObjectNormalToGround=true ; Optional, default true
```

Controls how often grass objects are placed, in this case, this places a grass object every 150 units. This is only known to work if it is < 512 units.

```ini
iGap=150 ; Required
```

### Placements

Each section can have different grass objects placed with differing probabilities. 

These configuration keys are suffixed by a index (a `#` placeholder is used below) starting from zero.
```ini
sChance#=100 ; Required
sMesh#=v7_GS_Grass_01.nif ; one of sID# or sMesh# is required
sID#= ; one of sID# or sMesh# is required
```

`sChance#` represents a weight, and the chances don't have to sum to 100

So for example

```ini
sChance0=50
sMesh0=v7_GS_Grass_leaf.nif

sChance1=10
sMesh1=v7_GS_Grass_fern.nif
```

### Exclusions

The randomization of the position of the groundcover can result it it spilling onto other textures. In some cases you don't want this to happen - for example with roads. Exclusions (or bans) can be used to stop this behaviour.

Don't place grass on `GL_Dirtroad`, even if random distribution puts it there
```ini
sBan#=GL_Dirtroad
```

The offset (default `0`) can be used to ensure that it isn't placed within some number of units of `GL_Dirtroad`
```ini
iBanOff#=128
```

## Troubleshooting

Logs are written to MwGroundcoverGenLog.txt. This includes details of
- The parsed configuration used
- What plugins where loaded
- The selector used and the actions taken for each cell

This can be useful to diagnose issues
