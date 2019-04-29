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
#include "util_gfx.h"

typedef struct {
    unsigned level;
    unsigned score;
    unsigned lives;
} NGame;

static NGame g_game;

void n_game_new(void)
{
    g_game.score = 0;
    g_game.lives = 0;

    n_hud_new();
}

void n_game_tick(void)
{
    n_hud_tick();
}

void n_game_draw(void)
{
    a_color_blendSet(A_COLOR_BLEND_PLAIN);
    a_sprite_blit(u_gfx_get(U_GFX_SCREEN), 0, 0);

    n_hud_draw();

    a_color_blendSet(A_COLOR_BLEND_MOD);
    a_sprite_blit(u_gfx_getNext(U_GFX_NOISE), 0, 0);
}
