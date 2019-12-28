/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_position.h"

struct ZCompPosition {
    AFix x, y;
};

size_t z_comp_position_size(void)
{
    return sizeof(ZCompPosition);
}

void z_comp_position_init(ZCompPosition* Position, AFix X, AFix Y)
{
    Position->x = X;
    Position->y = Y;
}

void z_comp_position_getCoordsFix(const ZCompPosition* Position, AFix* X, AFix* Y)
{
    *X = Position->x;
    *Y = Position->y;
}

void z_comp_position_getCoordsInt(const ZCompPosition* Position, int* X, int* Y)
{
    *X = a_fix_fixtoi(Position->x);
    *Y = a_fix_fixtoi(Position->y);
}

void z_comp_position_setCoords(ZCompPosition* Position, AFix X, AFix Y)
{
    Position->x = X;
    Position->y = Y;
}
