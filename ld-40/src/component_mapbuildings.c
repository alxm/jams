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

#include "component_mapbuildings.h"

typedef struct {
    AEntity* entity;
} ZBuildingTile;

struct ZCompMapBuildings {
    int w, h; // Map width and height
    ZBuildingTile** tiles; // [h][w]
    ZBuildingTile* tilesData; // [h * w]
};

size_t z_comp_mapbuildings_size(void)
{
    return sizeof(ZCompMapBuildings);
}

void z_comp_mapbuildings_init(ZCompMapBuildings* MapBuildings, const ZUtilLevel* Level)
{
    int w, h;
    z_util_level_getDim(Level, &w, &h);

    unsigned wu = (unsigned)w;
    unsigned hu = (unsigned)h;

    MapBuildings->w = w;
    MapBuildings->h = h;
    MapBuildings->tiles = a_mem_malloc(hu * sizeof(ZBuildingTile*));
    MapBuildings->tilesData = a_mem_malloc(hu * wu * sizeof(ZBuildingTile));

    for(int y = h; y--; ) {
        MapBuildings->tiles[y] = MapBuildings->tilesData + y * w;

        for(int x = w; x--; ) {
            MapBuildings->tiles[y][x].entity = NULL;
        }
    }
}

void z_comp_mapbuildings_free(void* Self)
{
    ZCompMapBuildings* buildings = Self;

    for(int y = buildings->h; y--; ) {
        for(int x = buildings->w; x--; ) {
            if(buildings->tiles[y][x].entity != NULL) {
                a_entity_release(buildings->tiles[y][x].entity);
            }
        }
    }

    free(buildings->tiles);
    free(buildings->tilesData);
}

AEntity* z_comp_mapbuildings_get(const ZCompMapBuildings* MapBuildings, int X, int Y)
{
    return MapBuildings->tiles[Y][X].entity;
}

void z_comp_mapbuildings_set(ZCompMapBuildings* MapBuildings, int X, int Y, AEntity* Building)
{
    if(MapBuildings->tiles[Y][X].entity != NULL) {
        a_entity_release(MapBuildings->tiles[Y][X].entity);
    }

    MapBuildings->tiles[Y][X].entity = Building;

    if(Building != NULL) {
        a_entity_reference(Building);
    }
}
