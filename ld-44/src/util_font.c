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

#include "util_font.h"

#include "util_color.h"
#include "util_gfx.h"

typedef struct {
    UColorId color;
    AFont* font;
} UFont;

static UFont g_fonts[U_FONT_NUM] = {
    [U_FONT_GRAY_LIGHT] = {.color = U_COLOR_GRAY_LIGHT},
    [U_FONT_GRAY_MEDIUM] = {.color = U_COLOR_GRAY_MEDIUM},
    [U_FONT_PINK] = {.color = U_COLOR_PINK},
    [U_FONT_PURPLE] = {.color = U_COLOR_PURPLE},
    [U_FONT_YELLOW] = {.color = U_COLOR_YELLOW},
};

void u_font_load(void)
{
    g_fonts[U_FONT_DEFAULT].font = a_font_newFromSprite(
                                u_gfx_get(U_GFX_FONT_DEFAULT), 0, 0);


    for(int i = 1; i < U_FONT_NUM; i++) {
        g_fonts[i].font = a_font_dup(g_fonts[U_FONT_DEFAULT].font,
                                     u_color_get(g_fonts[i].color));
    }
}

void u_font_unload(void)
{
    for(int i = U_FONT_NUM; i--; ) {
        a_font_free(g_fonts[i].font);
    }
}

const AFont* u_font_get(UFontId Id)
{
    return g_fonts[Id].font;
}

void u_font_int(int Number, int NumDigits, UFontId FontPadding, UFontId FontNumber, int X, int Y)
{
    a_font_coordsSet(X, Y);
    a_font_alignSet(A_FONT_ALIGN_LEFT);

    a_font_fontSet(u_font_get(FontPadding));
    a_font_printf("%0*d", NumDigits, Number);

    a_font_alignSet(A_FONT_ALIGN_RIGHT);
    a_font_fontSet(u_font_get(FontNumber));

    a_font_printf("%d", Number);
}
