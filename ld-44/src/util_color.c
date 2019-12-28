/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of Coffin Digital, a video game.

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

#include "util_color.h"

APixel g_colors[U_COLOR_NUM];

void u_color_load(void)
{
    ASprite* s = a_sprite_newFromPng("assets/gfx/palette.png");

    for(int x = a_math_min(a_sprite_sizeGetWidth(s), U_COLOR_NUM); x--; ) {
        g_colors[x] = a_sprite_pixelsGetPixel(s, x, 0);
    }

    a_sprite_free(s);
}

APixel u_color_get(UColorId Id)
{
    return g_colors[Id];
}
