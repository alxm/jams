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

#include "obj_hud.h"

#include "obj_game.h"
#include "util_color.h"
#include "util_font.h"

typedef struct {
    struct {
        int top[3];
        int bottom[3];
        int left[3];
        int right[3];
    } border;
} NHud;

static NHud g_hud;

void n_hud_new(void)
{
    memset(&g_hud, 0, sizeof(NHud));

    g_hud.border.top[1] = -1;
    g_hud.border.bottom[1] = -1;
    g_hud.border.left[1] = -1;
    g_hud.border.right[1] = -1;
}

void n_hud_tick(void)
{
    unsigned rate = a_fps_rateTickGet() / 4;

    if(a_fps_ticksNth(rate)) {
        g_hud.border.top[0] = a_random_range(43, 468);
        g_hud.border.top[1] = a_random_range(g_hud.border.top[0], 468);
        g_hud.border.top[2] = a_random_int(2);
    }

    if(a_fps_ticksNth(rate + rate / 4)) {
        g_hud.border.right[0] = a_random_range(28, 290);
        g_hud.border.right[1] = a_random_range(g_hud.border.right[0], 290);
        g_hud.border.right[2] = a_random_int(2);
    }

    if(a_fps_ticksNth(rate + rate / 2)) {
        g_hud.border.bottom[0] = a_random_range(43, 468);
        g_hud.border.bottom[1] = a_random_range(g_hud.border.bottom[0], 468);
        g_hud.border.bottom[2] = a_random_int(2);
    }

    if(a_fps_ticksNth(rate + rate * 3 / 4)) {
        g_hud.border.left[0] = a_random_range(28, 290);
        g_hud.border.left[1] = a_random_range(g_hud.border.left[0], 290);
        g_hud.border.left[2] = a_random_int(2);
    }
}

static void drawDate(void)
{
    a_font_coordsSet(371, 60);
    a_font_alignSet(A_FONT_ALIGN_LEFT);
    a_color_baseSetPixel(u_color_get(U_COLOR_GRAY_LIGHT));

    a_font_printf("SAT, Week 8");
}

static void drawBorder(void)
{
    a_color_blendSet(A_COLOR_BLEND_PLAIN);

    a_color_baseSetPixel(
        u_color_get(U_COLOR_GRAY_MEDIUM + g_hud.border.top[2]));
    a_draw_hline(g_hud.border.top[0], g_hud.border.top[1], 28);

    a_color_baseSetPixel(
        u_color_get(U_COLOR_GRAY_MEDIUM + g_hud.border.bottom[2]));
    a_draw_hline(g_hud.border.bottom[0], g_hud.border.bottom[1], 290);

    a_color_baseSetPixel(
        u_color_get(U_COLOR_GRAY_MEDIUM + g_hud.border.left[2]));
    a_draw_vline(43, g_hud.border.left[0], g_hud.border.left[1]);

    a_color_baseSetPixel(
        u_color_get(U_COLOR_GRAY_MEDIUM + g_hud.border.right[2]));
    a_draw_vline(468, g_hud.border.right[0], g_hud.border.right[1]);
}

void n_hud_draw(void)
{
    a_color_fillBlitSet(true);
    a_font_fontSet(u_font_get(U_FONT_DEFAULT));

    u_font_int(n_game_resourceGet(Z_RESOURCE_TRIANGLE), 4, 140, 60);
    u_font_int(n_game_resourceGet(Z_RESOURCE_SQUARE), 4, 183, 60);
    u_font_int(n_game_resourceGet(Z_RESOURCE_CIRCLE), 4, 226, 60);
    u_font_int(n_game_creditsGet(), 6, 268, 60);

    drawDate();

    a_color_fillBlitSet(false);

    drawBorder();
}
