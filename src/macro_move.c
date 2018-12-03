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

    c_position_coordsSet(position, coords);
    c_position_directionSet(position, Direction);
}
