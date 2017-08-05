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

#include "component_map.h"

#define Z_TILE_DIM 8

void z_system_mapTick(AEntity* Entity)
{
    A_UNUSED(Entity);
}

void z_system_mapDraw(AEntity* Entity)
{
    a_pixel_setHex(0x111122);
    a_draw_fill();

    ZCompMap* map = a_entity_getComponent(Entity, "map");

    int w, h;
    z_comp_map_getDim(map, &w, &h);

    for(int y = h; y--; ) {
        for(int x = w; x--; ) {
            if(z_comp_map_isWalkable(map, x, y)) {
                a_pixel_setHex(0xeeeedd);
            } else {
                a_pixel_setHex(0x448822);
            }
            a_draw_rectangle(x * Z_TILE_DIM,
                             y * Z_TILE_DIM,
                             Z_TILE_DIM,
                             Z_TILE_DIM);
        }
    }
}
