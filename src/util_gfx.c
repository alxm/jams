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

#include "util_gfx.h"

typedef struct {
    ASprite* sprite;
} UGfx;

static UGfx g_sprites[U_GFX_NUM];

static const char* g_paths[U_GFX_NUM] = {
    [U_GFX_SCREEN] = "screen.png",
};

static void gfxLoad(UGfxId Id)
{
    g_sprites[Id].sprite = a_sprite_newFromPng(
                            a_str_fmt512("assets/gfx/%s", g_paths[Id]));
}

static void gfxFree(UGfxId Id)
{
    a_sprite_free(g_sprites[Id].sprite);
}

void u_gfx_load(void)
{
    for(int i = U_GFX_NUM; i--; ) {
        gfxLoad(i);
    }
}

void u_gfx_unload(void)
{
    for(int i = U_GFX_NUM; i--; ) {
        gfxFree(i);
    }
}
