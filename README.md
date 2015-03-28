defullbright v0.1
=================

Quake texture tool for removing fullbright pixels.

Usage: `defullbright [-preview] wadfile1.wad [wadfile2.wad] [...]`

The default mode will save a copy of each of the provided wads with the suffix `-defullbright.wad`. Fullbright pixels in all textures (at all mip-levels) are swapped with the closest non-fullbright color.

With the `-preview` flag, the tool will generate previews (one .tga file per texture, saved to the current directory) of how textures look at a dim light level (33%) so you can visually check for fullbright pixels. No de-fullbrighting is performed.

NOTE: WAD3 (Half-Life) wads are also supported; the palette for all textures is assumed to be the Quake palette.

Credits
-------

by Eric Wasylishen <ewasylishen@gmail.com>; but most of the code is from lmp2pcx by LordHavoc, under the GPLv2+ license.