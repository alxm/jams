/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_tiles.h"

#include "component_map.h"

typedef struct ZTile {
    const ZUtilTile* utilTile;
} ZTile;

struct ZCompMap {
    int w, h;
    ZTile** tiles; // [height][width]
    ZTile* tilesData; // [height * width]
};

size_t z_comp_map_size(void)
{
    return sizeof(ZCompMap);
}

void z_comp_map_init(ZCompMap* Map, ASprite* Data)
{
    Map->w = a_sprite_getWidth(Data);
    Map->h = a_sprite_getHeight(Data);
    Map->tiles = a_mem_malloc((unsigned)Map->h * sizeof(ZTile*));
    Map->tilesData = a_mem_malloc((unsigned)Map->h * (unsigned)Map->w
                                    * sizeof(ZTile));

    for(int i = Map->h; i--; ) {
        Map->tiles[i] = Map->tilesData + i * Map->w;

        for(int j = Map->w; j--; ) {
            ZTile* tile = &Map->tiles[i][j];
            APixel pixel = a_sprite_getPixel(Data, j, i);

            if(pixel == 0) {
                // Not walkable
                tile->utilTile = z_util_tiles_getTile(0);
            } else {
                // Walkable
                tile->utilTile = z_util_tiles_getTile(1);
            }
        }
    }
}

void z_comp_map_free(void* Self)
{
    ZCompMap* map = Self;

    free(map->tiles);
    free(map->tilesData);
}

void z_comp_map_getDim(const ZCompMap* Map, int* Width, int* Height)
{
    *Width = Map->w;
    *Height = Map->h;
}

ASprite* z_comp_map_getSprite(const ZCompMap* Map, int X, int Y)
{
    return z_util_tiles_getSprite(Map->tiles[Y][X].utilTile);
}

bool z_comp_map_isWalkable(const ZCompMap* Map, int X, int Y)
{
    return z_util_tiles_isWalkable(Map->tiles[Y][X].utilTile);
}
