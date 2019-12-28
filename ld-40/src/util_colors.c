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

#include "util_colors.h"

APixel g_colors[Z_UTIL_COLOR_NUM][Z_UTIL_COLOR_LEVELS];

void z_util_colors_load(void)
{
    ASprite* palette = a_sprite_newFromFile("gfx/palette.png");

    for(ZUtilColor c = 0; c < Z_UTIL_COLOR_NUM; c++) {
        for(int l = 0; l < Z_UTIL_COLOR_LEVELS; l++) {
            g_colors[c][l] = a_sprite_getPixel(palette, l, 1 + c);
        }
    }

    a_sprite_free(palette);
}

APixel z_util_colors_get(ZUtilColor Color, int Level)
{
    return g_colors[Color][Level];
}
