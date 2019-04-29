/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of Coffin Digital, a video hud.

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

#include "obj_hud.h"

#include "util_color.h"
#include "util_font.h"

typedef struct {
    unsigned level;
    unsigned score;
    unsigned lives;
} NHud;

static NHud g_hud;

void n_hud_new(void)
{
    g_hud.score = 0;
    g_hud.lives = 0;
}

void n_hud_tick(void)
{
    //
}

static void writePad(int Number, int NumDigits, int X, int Y)
{
    a_font_coordsSet(X, Y);
    a_font_alignSet(A_FONT_ALIGN_LEFT);
    a_color_baseSetPixel(u_color_get(U_COLOR_GRAY_MEDIUM));

    a_font_printf("%0*d", NumDigits, Number);

    a_font_alignSet(A_FONT_ALIGN_RIGHT);
    a_color_baseSetPixel(u_color_get(U_COLOR_GRAY_LIGHT));

    a_font_printf("%d", Number);
}

static void drawDate(void)
{
    a_font_coordsSet(371, 60);
    a_font_alignSet(A_FONT_ALIGN_LEFT);
    a_color_baseSetPixel(u_color_get(U_COLOR_GRAY_LIGHT));

    a_font_printf("SAT, Week 8");
}

void n_hud_draw(void)
{
    a_color_fillBlitSet(true);
    a_font_fontSet(u_font_get(U_FONT_DEFAULT));

    writePad(12, 4, 140, 60);
    writePad(40, 4, 183, 60);
    writePad(20, 4, 226, 60);
    writePad(4380, 6, 268, 60);

    drawDate();

    a_color_fillBlitSet(false);
}
