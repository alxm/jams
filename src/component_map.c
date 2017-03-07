/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_map.h"

#include "util_graphics.h"

struct ZCompMap {
    const ZGraphic* tileGfx;
    int tiles[Z_MAP_TILES_H][Z_MAP_TILES_W];
};

size_t z_comp_map_size(void)
{
    return sizeof(ZCompMap);
}

void z_comp_map_init(ZCompMap* Map)
{
    Map->tileGfx = z_graphics_get("spaceTiles");
    int numFrames = (int)z_graphics_numFrames(Map->tileGfx);

    for(int i = 0; i < Z_MAP_TILES_H; i++) {
        for(int j = 0; j < Z_MAP_TILES_W; j++) {
            Map->tiles[i][j] = a_random_int(numFrames);
        }
    }
}

ASprite* z_comp_map_getTileSprite(const ZCompMap* Map, int X, int Y)
{
    return z_graphics_getFrame(Map->tileGfx, (unsigned)Map->tiles[Y][X]);
}
