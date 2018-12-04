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

#include "util_font.h"

static AFont* g_fonts[U_FONT_NUM];

void u_font_load(void)
{
    g_fonts[U_FONT_RED] = a_font_newFromFile("assets/gfx/font_red.png");
    g_fonts[U_FONT_BLUE] = a_font_newFromFile("assets/gfx/font_blue.png");
}

void u_font_unload(void)
{
    for(UFontId f = 0; f < U_FONT_NUM; f++) {
        a_font_free(g_fonts[f]);
    }
}

const AFont* u_font_get(UFontId Id)
{
    return g_fonts[Id];
}
