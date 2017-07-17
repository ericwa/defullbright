defullbright
============

- Website: https://github.com/ericwa/defullbright
- License: GPLv2+

Quake texture tool for removing fullbright pixels in wad files.

Usage: `defullbright [-preview] [-list list.txt] wadfile1.wad [wadfile2.wad] [...]`

The default mode will save a copy of each of the provided wads with the suffix `-defullbright.wad`. Fullbright pixels in all textures (at all mip-levels) are swapped with the closest non-fullbright color.

With the `-list list.txt` you can specify only certain texture names to be de-fullbrighted. The `list.txt` file should be a text file with one texture name per line.

With the `-preview` flag, the tool will generate previews (one .tga file per texture that has fullbrights, saved to the current directory) of how textures look at a dim light level so you can visually check for fullbright pixels. Brightness of the fullbright pixels is boosted to make sure the stand out. No de-fullbrighting is performed.

The general workflow is first use:
```
defullbright.exe -preview my_wad.wad
```
then, inspect the .tga files. If they are all "accidental" fullbright pixels, process the whole wad with:
```
defullbright.exe my_wad.wad
```
Otherwise, make a list of the bad texture names and save it to a "bad.txt" file, and run:
```
defullbright.exe -list bad.txt my_wad.wad
```

NOTE: WAD3 (Half-Life) wads are also supported; the palette for all textures is assumed to be the Quake palette.

History
-------

- v0.2
    - Tweaked settings for `-preview` to make the fullbright pixels more visible
    - Added `-list` option
    - For fence textures (textures starting with `{`), index 255 is left as-is
- v0.1
    - Initial release

Credits
-------

by Eric Wasylishen <ewasylishen@gmail.com>; but most of the code is from lmp2pcx by LordHavoc, under the GPLv2+ license.