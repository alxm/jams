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

#include "util_map.h"

typedef struct {
    ASpriteFrames* frames;
} UTile;

typedef const UTile* UMapTile;

struct UMap {
    const char* name;
    AStrHash* tilesTable; // table of UTile
    int w, h;
    UMapTile** tiles; // [h][w]
    UMapTile* tilesData; // [h * w]
};

static UMap g_maps[U_MAP_ID_NUM];

#define Z_MAP_PREFIX "assets/maps"

static char* pixelToKey(APixel Pixel)
{
    int r, g, b;
    a_pixel_toRgb(Pixel, &r, &g, &b);

    static char buffer[7];
    snprintf(buffer, sizeof(buffer), "%02x%02x%02x", r, g, b);

    return buffer;
}

static UTile* tileNew(const UMap* Map, const char* File)
{
    UTile* t = a_mem_malloc(sizeof(UTile));

    char buffer[64];
    snprintf(buffer, sizeof(buffer), Z_MAP_PREFIX "/%s/%s", Map->name, File);

    t->frames = a_spriteframes_newFromFileGrid(buffer, 16, 16, 0);

    return t;
}

static void tileFree(UTile* Tile)
{
    a_spriteframes_free(Tile->frames, true);

    free(Tile);
}

const UTile* tileGet(const UMap* Map, APixel Color)
{
    const char* key = pixelToKey(Color);
    const UTile* t = a_strhash_get(Map->tilesTable, key);

    if(t == NULL) {
        a_out_fatal("Map %s: unknown tile color %s", Map->name, key);
    }

    return t;
}

static void loadTiles(const UMap* Map)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), Z_MAP_PREFIX "/%s/tiles.txt", Map->name);

    ABlock* tiles = a_block_new(buffer);

    A_LIST_ITERATE(a_block_getAll(tiles), const ABlock*, t) {
        const char* file = a_block_readString(t, 1);
        APixel color = a_block_readPixel(t, 2);

        a_strhash_add(Map->tilesTable, pixelToKey(color), tileNew(Map, file));
    }

    a_block_free(tiles);
}

static void mapNew(UMapId Id, const char* Name)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), Z_MAP_PREFIX "/%s/map.png", Name);

    ASprite* image = a_sprite_newFromFile(buffer);

    UMap* m = &g_maps[Id];

    m->name = Name;
    m->tilesTable = a_strhash_new();

    loadTiles(m);

    m->w = a_sprite_widthGet(image);
    m->h = a_sprite_heightGet(image);

    m->tiles = a_mem_malloc((unsigned)m->h * sizeof(UMapTile*));
    m->tilesData = a_mem_malloc(
                    (unsigned)m->h * (unsigned)m->w * sizeof(UMapTile));

    for(int y = m->h; y--; ) {
        m->tiles[y] = m->tilesData + y * m->w;

        for(int x = m->w; x--; ) {
            m->tiles[y][x] = tileGet(m, a_sprite_pixelsGetPixel(image, x, y));
        }
    }

    a_sprite_free(image);
}

static void mapFree(UMapId Id)
{
    UMap* m = &g_maps[Id];

    a_strhash_freeEx(m->tilesTable, (AFree*)tileFree);

    free(m->tiles);
    free(m->tilesData);
}

void u_map_load(void)
{
    mapNew(U_MAP_ID_CAVE, "cave");
}

void u_map_unload(void)
{
    for(UMapId id = U_MAP_ID_NUM; id--; ) {
        mapFree(id);
    }
}
