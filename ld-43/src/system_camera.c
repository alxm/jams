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

#include "system_camera.h"

#include "component_position.h"

#include "state_game.h"

#include "util_ecs.h"
#include "util_map.h"

static void s_camera(AEntity* Entity)
{
    TGame* game = a_entity_contextGet(Entity);
    AEntity* player = t_game_getPlayer(game);

    CPosition* position = a_entity_componentReq(Entity, U_COM_POSITION);
    AVectorInt coords = c_position_coordsGet(
                            a_entity_componentReq(player, U_COM_POSITION));

    coords.x *= U_TILE_DIM;
    coords.y *= U_TILE_DIM;

    c_position_coordsSet(position, coords);
}

void s_camera_register(int Index)
{
    a_system_new(Index, s_camera, NULL, false);
    a_system_add(Index, U_COM_CAMERA);
    a_system_add(Index, U_COM_POSITION);
}
