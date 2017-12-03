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

#include "util_terrain.h"

typedef enum {
    Z_WALKABLE = 1,
} ZTerrainFlag;

typedef struct {
    int chance;
    ASpriteFrames* frames;
} ZUtilTerrainInstance;

typedef struct ZTerrain {
    ZTerrainFlag flags;
    APixel color; // used by loading
    AList* instances; // list of ZUtilTerrainInstance
    int totalChances;
} ZTerrain;

static ZTerrain g_terrains[Z_UTIL_TERRAIN_NUM];

static void instance_new(ZUtilTerrainType Terrain, int Chance, const ASprite* Sheet, int X, int Y)
{
    unsigned ticksPerFrame = a_fps_msToFrames(800);
    ASpriteFrames* frames = a_spriteframes_new(Sheet, X, Y, ticksPerFrame);

    ZUtilTerrainInstance* instance = a_mem_malloc(sizeof(ZUtilTerrainInstance));

    instance->chance = Chance;
    instance->frames = frames;

    a_list_addLast(g_terrains[Terrain].instances, instance);
    g_terrains[Terrain].totalChances += Chance;
}

static void instance_free(ZUtilTerrainInstance* Instance)
{
    a_spriteframes_free(Instance->frames, true);

    free(Instance);
}

static void terrain_init(ZUtilTerrainType Terrain, ZTerrainFlag Flags, APixel Color)
{
    ZTerrain* terrain = &g_terrains[Terrain];

    terrain->flags = Flags;
    terrain->color = Color;
    terrain->instances = a_list_new();
    terrain->totalChances = 0;
}

static void terrain_free(ZUtilTerrainType Terrain)
{
    ZTerrain* terrain = &g_terrains[Terrain];

    a_list_freeEx(terrain->instances, (AFree*)instance_free);
}

void z_util_terrain_load(void)
{
    ASprite* sheet = a_sprite_newFromFile("gfx/tiles.png");

    terrain_init(Z_UTIL_TERRAIN_PLAIN,
                 Z_WALKABLE,
                 a_pixel_hex(0xc9c9c9));
    instance_new(Z_UTIL_TERRAIN_PLAIN, 2, sheet, 0, 0);
    instance_new(Z_UTIL_TERRAIN_PLAIN, 1, sheet, 17, 0);

    terrain_init(Z_UTIL_TERRAIN_BUMPS,
                 0,
                 a_pixel_hex(0x353535));
    instance_new(Z_UTIL_TERRAIN_BUMPS, 1, sheet, 0, 17);

    terrain_init(Z_UTIL_TERRAIN_BUBBLES,
                 Z_WALKABLE,
                 a_pixel_hex(0x7f7f7f));
    for(int i = 0; i < 8; i++) {
        instance_new(Z_UTIL_TERRAIN_BUBBLES,
                     i == 0 ? 21 : 1,
                     sheet,
                     i * 17,
                     34);
    }

    a_sprite_free(sheet);
}

void z_util_terrain_unload(void)
{
    for(ZUtilTerrainType t = Z_UTIL_TERRAIN_NUM; t--; ) {
        terrain_free(t);
    }
}

bool z_util_terrain_isWalkable(ZUtilTerrainType Type)
{
    return g_terrains[Type].flags & Z_WALKABLE;
}

ASpriteFrames* z_util_terrain_dupFrames(ZUtilTerrainType Type)
{
    const ZTerrain* terrain = &g_terrains[Type];
    const int rand = a_random_int(terrain->totalChances);
    int counter = 0;

    A_LIST_ITERATE(terrain->instances, ZUtilTerrainInstance*, instance) {
        counter += instance->chance;

        if(counter > rand) {
            return a_spriteframes_dup(instance->frames, false);
        }
    }

    return NULL;
}

APixel z_util_terrain_getColor(ZUtilTerrainType Type)
{
    return g_terrains[Type].color;
}

