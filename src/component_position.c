/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of City, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "component_position.h"

struct CPosition {
    AVectorFix coords;
};

const size_t c_position_size = sizeof(CPosition);

void c_position_init(CPosition* Position, AVectorFix Coords)
{
    Position->coords = Coords;
}

void c_position_free(void* Self)
{
    CPosition* position = Self;

    A_UNUSED(position);
}


AVectorFix c_position_coordsGet(const CPosition* Position)
{
    return Position->coords;
}
