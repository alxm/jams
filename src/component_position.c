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

#include "component_position.h"

#include "util_level.h"

struct CPosition {
    AVectorInt coords;
    CPositionDirection direction;
};

static void c_position_initWithData(void* Self, const void* Data, const void* Context)
{
    A_UNUSED(Data);

    CPosition* position = Self;
    const ULevelEntityContext* context = Context;

    position->coords = context->coords;
    position->direction = C_POSITION_DOWN;
}

void c_position_register(int Index)
{
    a_component_new(Index, "position", sizeof(CPosition), NULL, NULL);

    a_component_dataSet(Index,
                        1,
                        NULL,
                        NULL,
                        c_position_initWithData);
}

AVectorInt c_position_coordsGet(const CPosition* Position)
{
    return Position->coords;
}

void c_position_coordsSet(CPosition* Position, AVectorInt Coords)
{
    Position->coords = Coords;
}

CPositionDirection c_position_directionGet(const CPosition* Position)
{
    return Position->direction;
}

void c_position_directionSet(CPosition* Position, CPositionDirection Direction)
{
    Position->direction = Direction;
}
