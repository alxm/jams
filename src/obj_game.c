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

#include "obj_event.h"
#include "obj_hud.h"
#include "util_color.h"
#include "util_gfx.h"

#define Z_CREDITS_START 2000

typedef struct {
    int resources[Z_RESOURCE_NUM];
    int credits;
    int event;
} NGame;

static NGame g_game;

void n_game_new(void)
{
    memset(&g_game, 0, sizeof(NGame));

    g_game.credits = Z_CREDITS_START;

    n_event_new();
    n_hud_new();
    n_market_new();
}

void n_game_tick(void)
{
    n_event_tick();
    n_market_tick();
    n_hud_tick();
}

void n_game_draw(void)
{
    a_color_blendSet(A_COLOR_BLEND_PLAIN);
    a_sprite_blit(u_gfx_get(U_GFX_SCREEN), 0, 0);

    n_event_draw();
    n_market_draw();
    n_hud_draw();

    a_color_blendSet(A_COLOR_BLEND_MOD);
    a_sprite_blit(u_gfx_getNext(U_GFX_NOISE), 0, 0);
}

int n_game_resourceGet(ZResourceId Id)
{
    return g_game.resources[Id];
}

int n_game_creditsGet(void)
{
    return g_game.credits;
}
