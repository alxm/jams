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

#pragma once

#include <a2x.h>

#define Z_COORDS_PIXELS_PER_UNIT 16

static inline AFix z_coords_pixelsToUnits(int Pixels)
{
    return a_fix_fromInt(Pixels) / Z_COORDS_PIXELS_PER_UNIT;
}

static inline AVectorInt z_coords_unitsToPixels(AVectorFix Units)
{
    return (AVectorInt){a_fix_toInt(Units.x * Z_COORDS_PIXELS_PER_UNIT),
                        a_fix_toInt(Units.y * Z_COORDS_PIXELS_PER_UNIT)};
}
