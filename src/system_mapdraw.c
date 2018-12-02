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

#include "system_mapdraw.h"

#include "component_map.h"

#include "util_ecs.h"

static void s_mapdraw(AEntity* Entity)
{
    CMap* cmap = a_entity_componentReq(Entity, U_COM_MAP);
    const UMap* map = c_map_mapGet(cmap);
    AVectorInt dim = u_map_dimGet(map);

    A_UNUSED(dim);

    a_screen_clear();

    for(int y = 0; y < dim.y; y++) {
        for(int x = 0; x < dim.x; x++) {
            const UTile* tile = u_map_tileGet(map, x, y);
            const ASprite* sprite = u_tile_spriteGet(tile);

            a_sprite_blit(sprite, x * U_TILE_DIM, y * U_TILE_DIM);
        }
    }
}

void s_mapdraw_register(int Index)
{
    a_system_new(Index, s_mapdraw, NULL, false);
    a_system_add(Index, U_COM_MAP);
}
