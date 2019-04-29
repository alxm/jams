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

#include "obj_game.h"

#include "obj_hud.h"
#include "util_color.h"
#include "util_gfx.h"

typedef struct {
    int resources[Z_RESOURCE_NUM];
    int credits;
    struct {
        int top[3];
        int bottom[3];
        int left[3];
        int right[3];
    } border;
} NGame;

static NGame g_game;

static void borderTick(void)
{
    unsigned rate = a_fps_rateTickGet() / 4;

    if(a_fps_ticksNth(rate)) {
        g_game.border.top[0] = a_random_range(43, 468);
        g_game.border.top[1] = a_random_range(g_game.border.top[0], 468);
        g_game.border.top[2] = a_random_int(2);
    }

    if(a_fps_ticksNth(rate + rate / 4)) {
        g_game.border.right[0] = a_random_range(28, 290);
        g_game.border.right[1] = a_random_range(g_game.border.right[0], 290);
        g_game.border.right[2] = a_random_int(2);
    }

    if(a_fps_ticksNth(rate + rate / 2)) {
        g_game.border.bottom[0] = a_random_range(43, 468);
        g_game.border.bottom[1] = a_random_range(g_game.border.bottom[0], 468);
        g_game.border.bottom[2] = a_random_int(2);
    }

    if(a_fps_ticksNth(rate + rate * 3 / 4)) {
        g_game.border.left[0] = a_random_range(28, 290);
        g_game.border.left[1] = a_random_range(g_game.border.left[0], 290);
        g_game.border.left[2] = a_random_int(2);
    }
}

static void borderDraw(void)
{
    a_color_blendSet(A_COLOR_BLEND_PLAIN);

    a_color_baseSetPixel(
        u_color_get(U_COLOR_GRAY_MEDIUM + g_game.border.top[2]));
    a_draw_hline(g_game.border.top[0], g_game.border.top[1], 28);

    a_color_baseSetPixel(
        u_color_get(U_COLOR_GRAY_MEDIUM + g_game.border.bottom[2]));
    a_draw_hline(g_game.border.bottom[0], g_game.border.bottom[1], 290);

    a_color_baseSetPixel(
        u_color_get(U_COLOR_GRAY_MEDIUM + g_game.border.left[2]));
    a_draw_vline(43, g_game.border.left[0], g_game.border.left[1]);

    a_color_baseSetPixel(
        u_color_get(U_COLOR_GRAY_MEDIUM + g_game.border.right[2]));
    a_draw_vline(468, g_game.border.right[0], g_game.border.right[1]);
}

void n_game_new(void)
{
    memset(&g_game, 0, sizeof(NGame));

    g_game.border.top[1] = -1;
    g_game.border.bottom[1] = -1;
    g_game.border.left[1] = -1;
    g_game.border.right[1] = -1;

    n_hud_new();
}

void n_game_tick(void)
{
    borderTick();
    n_hud_tick();
}

void n_game_draw(void)
{
    a_color_blendSet(A_COLOR_BLEND_PLAIN);
    a_sprite_blit(u_gfx_get(U_GFX_SCREEN), 0, 0);

    n_hud_draw();

    a_color_blendSet(A_COLOR_BLEND_MOD);
    a_sprite_blit(u_gfx_getNext(U_GFX_NOISE), 0, 0);

    borderDraw();
}

int n_game_resourceGet(ZResource Id)
{
    return g_game.resources[Id];
}

int n_game_creditsGet(void)
{
    return g_game.credits;
}
