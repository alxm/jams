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

#include "entity_camera.h"

#include "state_game.h"

#include "util_ecs.h"

static void s_mapdraw(AEntity* Entity)
{
    TGame* game = a_entity_contextGet(Entity);
    AEntity* camera = t_game_getCamera(game);

    CMap* cmap = a_entity_componentReq(Entity, U_COM_MAP);
    const UMap* umap = c_map_mapGet(cmap);
    AVectorInt dim = u_map_dimGet(umap);

    for(int y = 0; y < dim.y; y++) {
        for(int x = 0; x < dim.x; x++) {
            const UTile* tile = u_map_tileGet(umap, x, y);
            const ASprite* sprite = u_tile_spriteGet(tile);

            AVectorInt coords = {x * U_TILE_DIM, y * U_TILE_DIM};
            coords = e_camera_worldToScreen(camera, coords);

            a_sprite_blit(sprite, coords.x, coords.y);
        }
    }
}

void s_mapdraw_register(int Index)
{
    a_system_new(Index, s_mapdraw, NULL, false);
    a_system_add(Index, U_COM_MAP);
}
