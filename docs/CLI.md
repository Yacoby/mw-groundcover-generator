# Morrowind Groundcover Generator CLI

This currently takes a single action "generate" as the initial positional argument

### Generate

Arguments:
- configuration - path to the configuration file
- plugin - ordered list of esp/esm files to use. Can be specified multiple times to specify multiple plugins
- output - the path to the generated output file (could be overwritten)
- seed (optional) - the seed used for pseudo-randomness. Use if you want a consistent output 

#### Example

```
MWGroundcoverGeneratorCLI generate --configuration path/to/ini --plugin foo.esp --plugin bar.esp --output output.esp 
```


