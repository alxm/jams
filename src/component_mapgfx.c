/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_level.h"
#include "util_terrain.h"

#include "component_mapgfx.h"

typedef struct ZGfxTile {
    ASpriteFrames* frames;
} ZGfxTile;

struct ZCompMapGfx {
    int w, h; // Map width and height
    ZGfxTile** tiles; // [h][w]
    ZGfxTile* tilesData; // [h * w]
};

size_t z_comp_mapgfx_size(void)
{
    return sizeof(ZCompMapGfx);
}

void z_comp_mapgfx_init(ZCompMapGfx* MapGfx, const ZUtilTerrainType** Terrain, const ZUtilLevel* Level)
{
    int w, h;
    z_util_level_getDim(Level, &w, &h);

    unsigned wu = (unsigned)w;
    unsigned hu = (unsigned)h;

    MapGfx->w = w;
    MapGfx->h = h;
    MapGfx->tiles = a_mem_malloc(hu * sizeof(ZGfxTile*));
    MapGfx->tilesData = a_mem_malloc(hu * wu * sizeof(ZGfxTile));

    for(int y = h; y--; ) {
        MapGfx->tiles[y] = MapGfx->tilesData + y * w;

        for(int x = w; x--; ) {
            ZGfxTile* tile = &MapGfx->tiles[y][x];
            tile->frames = z_util_terrain_dupFrames(Terrain[y][x]);

            for(unsigned i = a_random_intu(a_fps_msToFrames(800)); i--; ) {
                a_spriteframes_next(tile->frames);
            }
        }
    }
}

void z_comp_mapgfx_free(void* Self)
{
    ZCompMapGfx* gfx = Self;

    for(int y = gfx->h; y--; ) {
        for(int x = gfx->w; x--; ) {
            ZGfxTile* tile = &gfx->tiles[y][x];
            a_spriteframes_free(tile->frames, false);
        }
    }

    free(gfx->tiles);
    free(gfx->tilesData);
}

ASprite* z_comp_mapgfx_getSprite(const ZCompMapGfx* MapGfx, int X, int Y)
{
    return a_spriteframes_getCurrent(MapGfx->tiles[Y][X].frames);
}

void z_comp_mapgfx_tickFrame(const ZCompMapGfx* MapGfx)
{
    for(int y = MapGfx->h; y--; ) {
        for(int x = MapGfx->w; x--; ) {
            ZGfxTile* tile = &MapGfx->tiles[y][x];
            a_spriteframes_next(tile->frames);
        }
    }
}
