/*
    Copyright 2016 Alex Margarit

    This file is part of Church 2, a game prototype made for the
    Cool Jams Inc 48h game jam that ran from 2016-12-02 to 2016-12-04.

    Church 2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Church 2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Church 2.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_map.h"
#include "tiles.h"

struct ZCompMap {
    int w, h;
    ZTileType** tiles;
};

size_t z_comp_map_size(void)
{
    return sizeof(ZCompMap);
}

void z_comp_map_free(void* Self)
{
    ZCompMap* m = Self;

    for(int i = m->h; i--; ) {
        free(m->tiles[i]);
    }

    free(m->tiles);
}

void z_comp_map_init(ZCompMap* Map)
{
    #define MAP_WIDTH 16
    #define MAP_HEIGHT 10

    ZTileType map[MAP_HEIGHT][MAP_WIDTH] = {
        {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,},
        {0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,},
        {0,0,0,0,4,4,4,3,3,4,4,4,0,0,0,0,},
        {0,0,0,0,0,0,4,0,0,4,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,},
    };

    Map->w = MAP_WIDTH;
    Map->h = MAP_HEIGHT;
    Map->tiles = a_mem_malloc(Map->h * sizeof(ZTileType*));

    for(int i = Map->h; i--;) {
        Map->tiles[i] = a_mem_malloc(Map->w * sizeof(ZTileType));

        for(int j = Map->w; j--; ) {
            ZTileType tile = map[i][j];

            if(tile == Z_TILE_TYPE_GROUND) {
                tile = Z_TILE_TYPE_PEBBLES;
            }

            if(tile == Z_TILE_TYPE_PEBBLES) {
                if(a_random_int(10) == 0) {
                    tile = Z_TILE_TYPE_ROCK;
                }
            }

            Map->tiles[i][j] = tile;
        }
    }
}

ASprite* z_comp_map_sprite(const ZCompMap* Map, int X, int Y)
{
    return z_tiles_sprite(Map->tiles[Y][X]);
}

void z_comp_map_getDim(const ZCompMap* Map, int* W, int* H)
{
    *W = Map->w;
    *H = Map->h;
}

bool z_comp_map_canWalk(const ZCompMap* Map, int X, int Y)
{
    if(X < 0 || Y < 0) {
        return false;
    }

    X /= Z_TILE_DIM;
    Y /= Z_TILE_DIM;

    if(X >= Map->w || Y >= Map->h) {
        return false;
    }

    return z_tiles_isWalkable(Map->tiles[Y][X]);
}
