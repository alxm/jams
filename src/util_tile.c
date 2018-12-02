/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Cave Shrine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cave Shrine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cave Shrine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "util_tile.h"

struct UTile {
    ASpriteFrames* frames;
};

static AStrHash* g_tiles; // table of UTile

#define Z_ASSETS_PREFIX "assets/maps"

static char* pixelToKey(APixel Pixel)
{
    int r, g, b;
    a_pixel_toRgb(Pixel, &r, &g, &b);

    static char buffer[7];
    snprintf(buffer, sizeof(buffer), "%02x%02x%02x", r, g, b);

    return buffer;
}

static UTile* tileNew(const char* File)
{
    UTile* t = a_mem_malloc(sizeof(UTile));

    char buffer[64];
    snprintf(buffer, sizeof(buffer), Z_ASSETS_PREFIX "/%s", File);

    t->frames = a_spriteframes_newFromFileGrid(buffer, 16, 16, 0);

    return t;
}

static void tileFree(UTile* Tile)
{
    a_spriteframes_free(Tile->frames, true);

    free(Tile);
}

void u_tile_load(void)
{
    g_tiles = a_strhash_new();

    ABlock* tiles = a_block_new(Z_ASSETS_PREFIX "/tiles.txt");

    A_LIST_ITERATE(a_block_getAll(tiles), const ABlock*, t) {
        const char* file = a_block_readString(t, 1);
        APixel color = a_block_readPixel(t, 2);

        a_strhash_add(g_tiles, pixelToKey(color), tileNew(file));
    }

    a_block_free(tiles);
}

void u_tile_unload(void)
{
    a_strhash_freeEx(g_tiles, (AFree*)tileFree);
}

const UTile* u_tile_get(APixel Color)
{
    const char* key = pixelToKey(Color);
    const UTile* t = a_strhash_get(g_tiles, key);

    if(t == NULL) {
        a_out_fatal("Unknown tile color %s", key);
    }

    return t;
}
