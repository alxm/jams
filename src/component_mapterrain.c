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

#include "util_colors.h"
#include "util_level.h"
#include "util_terrain.h"

#include "component_mapterrain.h"

struct ZCompMapTerrain {
    int w, h; // Map width and height
    ZUtilTerrainType** tiles; // [h][w]
    ZUtilTerrainType* tilesData; // [h * w]
};

size_t z_comp_mapterrain_size(void)
{
    return sizeof(ZCompMapTerrain);
}

void z_comp_mapterrain_init(ZCompMapTerrain* MapTerrain, const ZUtilLevel* Level)
{
    int w, h;
    z_util_level_getDim(Level, &w, &h);

    unsigned wu = (unsigned)w;
    unsigned hu = (unsigned)h;

    MapTerrain->w = w;
    MapTerrain->h = h;
    MapTerrain->tiles = a_mem_malloc(hu * sizeof(ZUtilTerrainType*));
    MapTerrain->tilesData = a_mem_malloc(hu * wu * sizeof(ZUtilTerrainType));

    for(int i = h; i--; ) {
        MapTerrain->tiles[i] = MapTerrain->tilesData + i * w;

        for(int j = w; j--; ) {
            MapTerrain->tiles[i][j] = Z_UTIL_TERRAIN_INVALID;
        }
    }

    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            APixel p = z_util_level_getValue(Level, x, y);

            MapTerrain->tiles[y][x] = Z_UTIL_TERRAIN_PLAIN;

            for(ZUtilTerrainType t = 0; t < Z_UTIL_TERRAIN_NUM; t++) {
                if(p == z_util_terrain_getColor(t)) {
                    MapTerrain->tiles[y][x] = t;
                    break;
                }
            }
        }
    }
}

void z_comp_mapterrain_free(void* Self)
{
    ZCompMapTerrain* terrain = Self;

    free(terrain->tiles);
    free(terrain->tilesData);
}

const ZUtilTerrainType** z_comp_mapterrain_getMap(const ZCompMapTerrain* Terrain)
{
    return (const ZUtilTerrainType**)Terrain->tiles;
}

void z_comp_mapterrain_getDim(const ZCompMapTerrain* Terrain, int* W, int* H)
{
    *W = Terrain->w;
    *H = Terrain->h;
}

ZUtilTerrainType z_comp_mapterrain_getType(const ZCompMapTerrain* Terrain, int X, int Y)
{
    return Terrain->tiles[Y][X];
}
