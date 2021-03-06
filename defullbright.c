/*
Copyright (C) 2002-2007 Forest Hale
Copyright (C) 2014 Eric Wasylishen

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#define NUMFULLBRIGHTS 32

// list of texture names to process
static int texlist_count = 0;
#define TEXLIST_MAX 65536
static char texlist[TEXLIST_MAX][65];

static void Texlist_Add(const char *texname)
{
	if (texlist_count >= TEXLIST_MAX)
	{
		printf("-list overflow\n");
		exit(1);
	}
	if (strlen(texname) > 64)
	{
		printf("texture name must be 64 chars or less\n");
		exit(1);
	}

	strcpy(texlist[texlist_count], texname);
	texlist_count++;
}

static bool Texlist_Contains(const char *texname)
{
	for (int i=0; i<texlist_count; i++)
	{
		if (!strcmp(texlist[i], texname))
			return true;
	}
	return false;
}

unsigned char quakepalette[768] =
{
	0x00,0x00,0x00,0x0F,0x0F,0x0F,0x1F,0x1F,0x1F,0x2F,0x2F,0x2F,0x3F,0x3F,0x3F,0x4B,
	0x4B,0x4B,0x5B,0x5B,0x5B,0x6B,0x6B,0x6B,0x7B,0x7B,0x7B,0x8B,0x8B,0x8B,0x9B,0x9B,
	0x9B,0xAB,0xAB,0xAB,0xBB,0xBB,0xBB,0xCB,0xCB,0xCB,0xDB,0xDB,0xDB,0xEB,0xEB,0xEB,
	0x0F,0x0B,0x07,0x17,0x0F,0x0B,0x1F,0x17,0x0B,0x27,0x1B,0x0F,0x2F,0x23,0x13,0x37,
	0x2B,0x17,0x3F,0x2F,0x17,0x4B,0x37,0x1B,0x53,0x3B,0x1B,0x5B,0x43,0x1F,0x63,0x4B,
	0x1F,0x6B,0x53,0x1F,0x73,0x57,0x1F,0x7B,0x5F,0x23,0x83,0x67,0x23,0x8F,0x6F,0x23,
	0x0B,0x0B,0x0F,0x13,0x13,0x1B,0x1B,0x1B,0x27,0x27,0x27,0x33,0x2F,0x2F,0x3F,0x37,
	0x37,0x4B,0x3F,0x3F,0x57,0x47,0x47,0x67,0x4F,0x4F,0x73,0x5B,0x5B,0x7F,0x63,0x63,
	0x8B,0x6B,0x6B,0x97,0x73,0x73,0xA3,0x7B,0x7B,0xAF,0x83,0x83,0xBB,0x8B,0x8B,0xCB,
	0x00,0x00,0x00,0x07,0x07,0x00,0x0B,0x0B,0x00,0x13,0x13,0x00,0x1B,0x1B,0x00,0x23,
	0x23,0x00,0x2B,0x2B,0x07,0x2F,0x2F,0x07,0x37,0x37,0x07,0x3F,0x3F,0x07,0x47,0x47,
	0x07,0x4B,0x4B,0x0B,0x53,0x53,0x0B,0x5B,0x5B,0x0B,0x63,0x63,0x0B,0x6B,0x6B,0x0F,
	0x07,0x00,0x00,0x0F,0x00,0x00,0x17,0x00,0x00,0x1F,0x00,0x00,0x27,0x00,0x00,0x2F,
	0x00,0x00,0x37,0x00,0x00,0x3F,0x00,0x00,0x47,0x00,0x00,0x4F,0x00,0x00,0x57,0x00,
	0x00,0x5F,0x00,0x00,0x67,0x00,0x00,0x6F,0x00,0x00,0x77,0x00,0x00,0x7F,0x00,0x00,
	0x13,0x13,0x00,0x1B,0x1B,0x00,0x23,0x23,0x00,0x2F,0x2B,0x00,0x37,0x2F,0x00,0x43,
	0x37,0x00,0x4B,0x3B,0x07,0x57,0x43,0x07,0x5F,0x47,0x07,0x6B,0x4B,0x0B,0x77,0x53,
	0x0F,0x83,0x57,0x13,0x8B,0x5B,0x13,0x97,0x5F,0x1B,0xA3,0x63,0x1F,0xAF,0x67,0x23,
	0x23,0x13,0x07,0x2F,0x17,0x0B,0x3B,0x1F,0x0F,0x4B,0x23,0x13,0x57,0x2B,0x17,0x63,
	0x2F,0x1F,0x73,0x37,0x23,0x7F,0x3B,0x2B,0x8F,0x43,0x33,0x9F,0x4F,0x33,0xAF,0x63,
	0x2F,0xBF,0x77,0x2F,0xCF,0x8F,0x2B,0xDF,0xAB,0x27,0xEF,0xCB,0x1F,0xFF,0xF3,0x1B,
	0x0B,0x07,0x00,0x1B,0x13,0x00,0x2B,0x23,0x0F,0x37,0x2B,0x13,0x47,0x33,0x1B,0x53,
	0x37,0x23,0x63,0x3F,0x2B,0x6F,0x47,0x33,0x7F,0x53,0x3F,0x8B,0x5F,0x47,0x9B,0x6B,
	0x53,0xA7,0x7B,0x5F,0xB7,0x87,0x6B,0xC3,0x93,0x7B,0xD3,0xA3,0x8B,0xE3,0xB3,0x97,
	0xAB,0x8B,0xA3,0x9F,0x7F,0x97,0x93,0x73,0x87,0x8B,0x67,0x7B,0x7F,0x5B,0x6F,0x77,
	0x53,0x63,0x6B,0x4B,0x57,0x5F,0x3F,0x4B,0x57,0x37,0x43,0x4B,0x2F,0x37,0x43,0x27,
	0x2F,0x37,0x1F,0x23,0x2B,0x17,0x1B,0x23,0x13,0x13,0x17,0x0B,0x0B,0x0F,0x07,0x07,
	0xBB,0x73,0x9F,0xAF,0x6B,0x8F,0xA3,0x5F,0x83,0x97,0x57,0x77,0x8B,0x4F,0x6B,0x7F,
	0x4B,0x5F,0x73,0x43,0x53,0x6B,0x3B,0x4B,0x5F,0x33,0x3F,0x53,0x2B,0x37,0x47,0x23,
	0x2B,0x3B,0x1F,0x23,0x2F,0x17,0x1B,0x23,0x13,0x13,0x17,0x0B,0x0B,0x0F,0x07,0x07,
	0xDB,0xC3,0xBB,0xCB,0xB3,0xA7,0xBF,0xA3,0x9B,0xAF,0x97,0x8B,0xA3,0x87,0x7B,0x97,
	0x7B,0x6F,0x87,0x6F,0x5F,0x7B,0x63,0x53,0x6B,0x57,0x47,0x5F,0x4B,0x3B,0x53,0x3F,
	0x33,0x43,0x33,0x27,0x37,0x2B,0x1F,0x27,0x1F,0x17,0x1B,0x13,0x0F,0x0F,0x0B,0x07,
	0x6F,0x83,0x7B,0x67,0x7B,0x6F,0x5F,0x73,0x67,0x57,0x6B,0x5F,0x4F,0x63,0x57,0x47,
	0x5B,0x4F,0x3F,0x53,0x47,0x37,0x4B,0x3F,0x2F,0x43,0x37,0x2B,0x3B,0x2F,0x23,0x33,
	0x27,0x1F,0x2B,0x1F,0x17,0x23,0x17,0x0F,0x1B,0x13,0x0B,0x13,0x0B,0x07,0x0B,0x07,
	0xFF,0xF3,0x1B,0xEF,0xDF,0x17,0xDB,0xCB,0x13,0xCB,0xB7,0x0F,0xBB,0xA7,0x0F,0xAB,
	0x97,0x0B,0x9B,0x83,0x07,0x8B,0x73,0x07,0x7B,0x63,0x07,0x6B,0x53,0x00,0x5B,0x47,
	0x00,0x4B,0x37,0x00,0x3B,0x2B,0x00,0x2B,0x1F,0x00,0x1B,0x0F,0x00,0x0B,0x07,0x00,
	0x00,0x00,0xFF,0x0B,0x0B,0xEF,0x13,0x13,0xDF,0x1B,0x1B,0xCF,0x23,0x23,0xBF,0x2B,
	0x2B,0xAF,0x2F,0x2F,0x9F,0x2F,0x2F,0x8F,0x2F,0x2F,0x7F,0x2F,0x2F,0x6F,0x2F,0x2F,
	0x5F,0x2B,0x2B,0x4F,0x23,0x23,0x3F,0x1B,0x1B,0x2F,0x13,0x13,0x1F,0x0B,0x0B,0x0F,
	0x2B,0x00,0x00,0x3B,0x00,0x00,0x4B,0x07,0x00,0x5F,0x07,0x00,0x6F,0x0F,0x00,0x7F,
	0x17,0x07,0x93,0x1F,0x07,0xA3,0x27,0x0B,0xB7,0x33,0x0F,0xC3,0x4B,0x1B,0xCF,0x63,
	0x2B,0xDB,0x7F,0x3B,0xE3,0x97,0x4F,0xE7,0xAB,0x5F,0xEF,0xBF,0x77,0xF7,0xD3,0x8B,
	0xA7,0x7B,0x3B,0xB7,0x9B,0x37,0xC7,0xC3,0x37,0xE7,0xE3,0x57,0x7F,0xBF,0xFF,0xAB,
	0xE7,0xFF,0xD7,0xFF,0xFF,0x67,0x00,0x00,0x8B,0x00,0x00,0xB3,0x00,0x00,0xD7,0x00,
	0x00,0xFF,0x00,0x00,0xFF,0xF3,0x93,0xFF,0xF7,0xC7,0xFF,0xFF,0xFF,0x9F,0x5B,0x53
};

static bool IsFullbright(unsigned char index)
{
	return index >= (256 - NUMFULLBRIGHTS);
}

#define qmin(a, b) ((a) < (b) ? (a) : (b))

double Dist(int r1, int g1, int b1, int r2, int g2, int b2)
{
	int deltar = r1 - r2;
	int deltag = g1 - g2;
	int deltab = b1 - b2;
	return sqrt((deltar * deltar) 
		+ (deltag * deltag)
		+ (deltab * deltab));
}

unsigned char DeFullbright(unsigned char in)
{
        static int builtfixup;
        static unsigned char fixuptable[NUMFULLBRIGHTS];
        if (!builtfixup)
        {
                int i;
                for (i=0; i<NUMFULLBRIGHTS; i++)
                {
                        int brightindex;
                        int r, g, b;

                        brightindex = 256 - NUMFULLBRIGHTS + i;
                        r = quakepalette[(3*brightindex)];
                        g = quakepalette[(3*brightindex)+1];
                        b = quakepalette[(3*brightindex)+2];

                        // find closest non-fullbright index
                        double bestdist = 1000000000;
                        int bestindex = -1;
                        int j;
                        for (j=0; j<256 - NUMFULLBRIGHTS; j++)
                        {
                                int rj, gj, bj;
                                rj = quakepalette[(3*j)];
                                gj = quakepalette[(3*j)+1];
                                bj = quakepalette[(3*j)+2];

                                double dist = Dist(r, g, b, rj, gj, bj);
                                if (dist < bestdist)
                                {
                                        bestdist = dist;
                                        bestindex = j;
                                }
                        }
			assert(bestindex != -1);
                        fixuptable[i] = bestindex;
                }
                builtfixup = 1;
        }

        if (!IsFullbright(in))
                return in;
        else
                return fixuptable[(int)in - (256 - NUMFULLBRIGHTS)];
}

static bool HasFullbrights(const unsigned char *data, const int count)
{
	for (int p=0; p<count; p++)
	{
		if (IsFullbright(data[p]))
			return true;
	}
	return false;
}

void DeFullbrightPixels(unsigned char *data, const int count, const bool preserve_255)
{
	for (int p=0; p<count; p++)
	{
		if (!preserve_255 || data[p] != 255)
			data[p] = DeFullbright(data[p]);
	}
}

int readfile(const char *filename, void **mem, int *size)
{
	FILE *file;
	char *filemem;
	int filesize, readsize;

	*mem = NULL;
	*size = 0;
	file = fopen(filename, "rb");
	if (!file)
		return -1;

	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	filemem = malloc(filesize);
	if (!filemem)
	{
		fclose(file);
		return -1;
	}

	readsize = fread(filemem, 1, filesize, file);
	fclose(file);

	if (readsize < filesize)
	{
		free(filemem);
		return -1;
	}

	*mem = filemem;
	*size = filesize;
	return 0;
}

int writefile(char *filename, void *mem, int size)
{
	FILE *file;
	int writesize;
	file = fopen(filename, "wb");
	if (!file)
		return -1;
	writesize = fwrite(mem, 1, size, file);
	fclose(file);
	if (writesize < size)
		return -2;
	return 0;
}

int image_width, image_height;

int NoSwapLong(int num)
{
	return num;
}

int SwapLong(int num)
{
	return (((num >>  0) & 0xFF) << 24) + (((num >>  8) & 0xFF) << 16) + (((num >> 16) & 0xFF) << 8) + (((num >> 24) & 0xFF) << 0);
}

short NoSwapShort(short num)
{
	return num;
}

short SwapShort(short num)
{
	return (((num >>  0) & 0xFF) << 8) + (((num >>  8) & 0xFF) << 0);
}

int (*LittleLong)(int num);
int (*BigLong)(int num);
short (*LittleShort)(short num);
short (*BigShort)(short num);

void SwapDetect()
{
	unsigned char t[4] = {0, 0, 0, 1};
	int *test = (int *) t;
	if (*test == 1) // big endian
	{
		LittleLong = SwapLong;
		BigLong = NoSwapLong;
		LittleShort = SwapShort;
		BigShort = NoSwapShort;
	}
	else // little endian
	{
		LittleLong = NoSwapLong;
		BigLong = SwapLong;
		LittleShort = NoSwapShort;
		BigShort = SwapShort;
	}
}

#define	CMP_NONE		0

#define	TYP_NONE		0
#define	TYP_LABEL		1

#define	TYP_LUMPY		64				// 64 + grab command number
#define	TYP_PALETTE		64
#define	TYP_QTEX		65
#define	TYP_QPIC		66
#define	TYP_SOUND		67
#define	TYP_MIPTEX		68

typedef struct
{
	int			width, height;
	unsigned char		data[4];			// variably sized
}
qpic_t;

typedef struct
{
	char		identification[4];		// should be WAD2 or 2DAW
	int			numlumps;
	int			infotableofs;
}
wadinfo_t;

typedef struct
{
	int			filepos;
	int			disksize;
	int			size;					// uncompressed
	char		type;
	char		compression;
	char		pad1, pad2;
	char		name[16];				// must be null terminated
}
lumpinfo_t;

#define MIPTEXNAME  16
#define MIPLEVELS   4
typedef struct miptex_s {
        char        name[MIPTEXNAME];
        uint32_t    width, height;
        uint32_t    offsets[MIPLEVELS];         // four mip maps stored
} miptex_t;

#define TYPE_WAD2 0
#define TYPE_WAD3 1

static int previews_written = 0;

void Preview(const char *texname, const unsigned char *data, int width, int height)
{
	char tempname[4096];
	sprintf(tempname, "%s.tga", texname);
	
	int y;
	const unsigned char *in, *end;
	unsigned char *buffer, *out;

	// contains transparent pixels
	// BGRA truecolor since some programs can't deal with BGRA colormaps
	// a buffer big enough to store the worst compression ratio possible (1 extra byte per pixel)
	buffer = malloc(18+width*height*5);

	memset (buffer, 0, 18);
	buffer[2] = 2; // uncompressed type
	buffer[12] = (width >> 0) & 0xFF;
	buffer[13] = (width >> 8) & 0xFF;
	buffer[14] = (height >> 0) & 0xFF;
	buffer[15] = (height >> 8) & 0xFF;
	buffer[16] = 32;	// pixel size
	buffer[17] = 8; // 8 attribute bits per pixel, bottom left origin
	out = buffer + 18;

	bool has_fullbrights = false;

	// copy image
	for (y = 0;y < height;y++)
	{
		in = data + (height - 1 - y) * width;
		end = in + width;
		while (in < end)
		{
			unsigned char pixel = *in;
			
			if (pixel == 255 && texname[0] == '{')
			{
				*out++ = 0;
				*out++ = 0;
				*out++ = 0;
				*out++ = 0;
			}
			else
			{
				if (IsFullbright(pixel))
					has_fullbrights = true;

				// Multiply fullbright pixels by 4 to make them brighter
				// Darken non-fullbright pixels by 50%
				float factor = IsFullbright(pixel) ? 4.0 : 0.5;
			
				*out++ = qmin(255, (int)(quakepalette[pixel*3+2] * factor));
				*out++ = qmin(255, (int)(quakepalette[pixel*3+1] * factor));
				*out++ = qmin(255, (int)(quakepalette[pixel*3+0] * factor));
				*out++ = 255;
			}
			in ++;
		}
	}

	// only bother if there are some fullbrights
	if (has_fullbrights)
	{
		if (0 == writefile(tempname, buffer, out - buffer))
			previews_written++;
	}

	free(buffer);
}

void defullbright(const char *filename, bool preview)
{
	char tempname[4096];
	int wadsize, size, numlumps, i;
	void *waddata;
	unsigned char *data;
	wadinfo_t *wad;
	lumpinfo_t *lump;
	int filetype;
	
	if (readfile(filename, &waddata, &wadsize))
	{
		printf("defullbright: could not load \"%s\"\n", filename);
		return;
	}
	wad = (void *)waddata;

	if (memcmp(wad->identification, "WAD2", 4) == 0)
		filetype = TYPE_WAD2;
	else if (memcmp(wad->identification, "WAD3", 4) == 0)
		filetype = TYPE_WAD3;
	else
	{
		printf("defullbright: \"%s\" is not a quake wad2 or wad3 file\n", filename);
		return;
	}

	numlumps = LittleLong(wad->numlumps);
	printf("defullbright: converting \"%s\" (%i lumps)\n", filename, numlumps);
	lump = (void *)((char *) waddata + wad->infotableofs);
	for (i = 0;i < numlumps;i++, lump++)
	{
		if (lump->compression != CMP_NONE)
		{
			printf("lump \"%s\" is compressed, compression is unsupported\n", lump->name);
			continue;
		}
		data = lump->filepos + (unsigned char *) waddata;
		size = lump->disksize;
	
		if (filetype == TYPE_WAD3 /* assume all WAD3 lumps are miptex's */  ||
		    (filetype == TYPE_WAD2 && lump->type == TYP_MIPTEX))
		{
			miptex_t *miptex = (miptex_t *) data;
			unsigned char *pixeldata = (unsigned char *)(miptex + 1);
			int w = LittleLong(miptex->width);
			int h = LittleLong(miptex->height);
			int pixeldata_numbytes = w * h / 64 * 85;
			int max_pixeldata_bytes = size - sizeof(miptex_t);

			// check for corrupt wads
			if (pixeldata_numbytes > max_pixeldata_bytes)
			{
				printf("warning: '%s' (%dx%d) should have %d bytes of pixel data, but the lump only has room for %d.\n",
					lump->name, w, h, pixeldata_numbytes, max_pixeldata_bytes);

				pixeldata_numbytes = max_pixeldata_bytes;
			}

//			printf("encountered lump type 'MIPTEX' named \"%s\" (%d x %d). defullbrighting %d bytes of %d lump\n",
//				 lump->name, w, h, pixeldata_numbytes, max_pixeldata_bytes);

			const bool in_texlist = Texlist_Contains(miptex->name);
			const bool has_fullbrights = HasFullbrights(pixeldata, pixeldata_numbytes);

			if (preview)
				Preview(lump->name, pixeldata, w, h);
			else
			{
				if (!has_fullbrights)
				{
					printf("skipping '%s' (no fullbright pixels)\n", miptex->name);
				}
				else if (!in_texlist && texlist_count != 0)
				{
					printf("skipping '%s' (not in -list file)\n", miptex->name);
				}
				else
				{
					const bool preserve_255 = (miptex->name[0] == '{');
			
					printf("applying de-fullbrighting to '%s'\n", miptex->name);

					DeFullbrightPixels(pixeldata, pixeldata_numbytes, preserve_255);
				}
			}
		}
		else
		{
			printf("encountered unknown lump type named \"%s\"\n", lump->name);
		}
	}
	
	
	if (!preview)
	{
		sprintf(tempname, "%s", filename);
		int filenamelen = strlen(filename);
		sprintf(tempname + filenamelen - (tempname[filenamelen - 4] == '.' ? 4 : 0), "-defullbright.wad");

		if (0 != writefile(tempname, waddata, wadsize))
		{
			printf("error writing '%s'\n", tempname);
		}
		else
		{
			printf("wrote de-fullbrighted wad to '%s'\n", tempname);
		}
	}
	
	free(waddata);
}

static void LoadList(const char *filename)
{
	printf("Loading list of textures to defullbright from '%s'...\n", filename);
	
	FILE *f = fopen(filename, "rt");
	if (!f)
	{
		printf("error opening '%s'\n", filename);
		exit(1);
	}

	char texname[65];
	int fields = 0;
	while ((fields = fscanf(f, "%64s", texname)) == 1)
	{
		Texlist_Add(texname);
	}
		
	fclose(f);
}

int main(int argc, char **argv)
{
	int i = 1;
	int preview = 0;

	SwapDetect();
	
	if (i < argc && !strcmp("-preview", argv[i]))
	{
		i++;
		preview = 1;
	}

	if (i < argc && !strcmp("-list", argv[i]))
	{
		i++;
		if (i >= argc)
		{
			printf("-list requires an argument\n");
			return 1;
		}
		LoadList(argv[i]);
		i++;
	}
	
	if (i >= argc)
	{
		printf("usage: defullbright [-preview] [-list list.txt] wadfile1.wad [wadfile2.wad] [...]\n");
		return 1;
	}

	for (; i<argc; i++)
		defullbright(argv[i], preview);

	if (preview)
		printf("wrote %d .tga previews of the textures containing fullbrights\n", previews_written);

	return 0;
}
