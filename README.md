This is an old project and so the code is pretty terrible

MW Mesh Generator 0.1.6
====================================================================

### License and copyright

Copyright (c) 2009 Jacob Essex

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.


### Introduction

This program allows you to place meshes over the Morrowind landscape
with spesific meshes being placed on spesific ground textures.


### Known issues

The settings editor is untested

Very little error checking is done. An invalid ini will screw things up

I am fairly sure generating your own containers will not work, even
though it is allowed on the settings editor

Cells with coordinates of 512 or greater won't work (or for that matter
-512 or lower)

### Installation instructions

Extract the files to any location.

### Using the software

If you need to edit any of the settings, either edit
them directly in the ini, or with the settings editor included.

The example inis included(grass\_cell\_ref.ini, grass\_general\_info.ini) contain
general information on the format of the ini

Run the exe.

Settings Location - The grass ini file included with the mod
Morrowind Location - This should be the location of your Morrowind dir.
Output file - What the new esp will be called (saved to your data files)


Press "Import from INI", or add mods manualy.

ID Append - The unquie ID appended to the front of all objects
Z offset - Increase or decrease the height of the meshes
Thread - Leave as it is unless the grass isn't generating.
			If you untick this, the user interface will freeze 
			while generating grass



Generate - The button to press to start

### Contributors 

* Vality7 - Huge amount of feedback/testing
* PirateLord - Feedback/Ideas
* zackg - Poking me about bugs
