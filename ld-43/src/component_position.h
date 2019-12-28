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

#pragma once

#include <a2x.h>

typedef struct CPosition CPosition;

typedef enum {
    C_POSITION_UP,
    C_POSITION_DOWN,
    C_POSITION_LEFT,
    C_POSITION_RIGHT,
    C_POSITION_NUM
} CPositionDirection;

extern void c_position_register(int Index);

extern AVectorInt c_position_coordsGet(const CPosition* Position);
extern void c_position_coordsSet(CPosition* Position, AVectorInt Coords);

extern CPositionDirection c_position_directionGet(const CPosition* Position);
extern void c_position_directionSet(CPosition* Position, CPositionDirection Direction);
