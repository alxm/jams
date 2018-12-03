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

#include "macro_move.h"

#include "component_map.h"

#include "state_game.h"

#include "util_ecs.h"

void m_move_direction(AEntity* Entity, CPositionDirection Direction)
{
    static const AVectorInt incs[C_POSITION_NUM] = {
        [C_POSITION_UP] = {0, -1},
        [C_POSITION_DOWN] = {0, 1},
        [C_POSITION_LEFT] = {-1, 0},
        [C_POSITION_RIGHT] = {1, 0},
    };

    CPosition* position = a_entity_componentReq(Entity, U_COM_POSITION);
    AVectorInt coords = c_position_coordsGet(position);

    coords.x += incs[Direction].x;
    coords.y += incs[Direction].y;

    m_move_coordsSet(Entity, coords);
    c_position_directionSet(position, Direction);
}

void m_move_coordsSet(AEntity* Entity, AVectorInt Coords)
{
    TGame* game = a_entity_contextGet(Entity);
    CMap* map = a_entity_componentReq(t_game_getMap(game), U_COM_MAP);

    CPosition* position = a_entity_componentReq(Entity, U_COM_POSITION);
    AVectorInt oldCoords = c_position_coordsGet(position);

    AEntity* otherEntity = c_map_entityGet(map, Coords);

    if(otherEntity) {
        a_entity_messageSend(otherEntity, Entity, U_MSG_INTERACT);
    } else {
        const UMap* umap = c_map_mapGet(map);
        const UTile* utile = u_map_getTile(umap, Coords.x, Coords.y);

        if(!u_tile_getBlock(utile)) {
            c_map_entitySet(map, oldCoords, NULL);
            c_map_entitySet(map, Coords, Entity);

            c_position_coordsSet(position, Coords);

            int code = u_tile_getCode(utile);

            if(code > 0) {
                t_game_runCode(game, code);
            }
        }
    }
}
