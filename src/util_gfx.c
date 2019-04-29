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
    const char* path;
    bool frames;
    union {
        ASprite* sprite;
        ASpriteFrames* frames;
    } u;
} UGfx;

static UGfx g_gfx[U_GFX_NUM] = {
    [U_GFX_FONT_DEFAULT] = {.path = "font_grid6x8.png"},
    [U_GFX_ICON_MSG] = {.path = "icon_msg.png"},
    [U_GFX_NOISE] = {.path = "noise_grid512x320.png", .frames = true},
    [U_GFX_SCREEN] = {.path = "screen.png"},
};

static void gfxLoad(UGfxId Id)
{
    if(g_gfx[Id].frames) {
        g_gfx[Id].u.frames = a_spriteframes_newFromPng(
                                a_str_fmt512("assets/gfx/%s", g_gfx[Id].path),
                                0,
                                0);
    } else {
        g_gfx[Id].u.sprite = a_sprite_newFromPng(
                                a_str_fmt512("assets/gfx/%s", g_gfx[Id].path));
    }
}

static void gfxFree(UGfxId Id)
{
    if(g_gfx[Id].frames) {
        a_spriteframes_free(g_gfx[Id].u.frames, true);
    } else {
        a_sprite_free(g_gfx[Id].u.sprite);
    }
}

void u_gfx_load(void)
{
    for(int i = U_GFX_NUM; i--; ) {
        gfxLoad(i);
    }

    a_spriteframes_speedSet(g_gfx[U_GFX_NOISE].u.frames, A_TIMER_MS, 150);
}

void u_gfx_unload(void)
{
    for(int i = U_GFX_NUM; i--; ) {
        gfxFree(i);
    }
}

const ASprite* u_gfx_get(UGfxId Id)
{
    if(g_gfx[Id].frames) {
        return a_spriteframes_getCurrent(g_gfx[Id].u.frames);
    } else {
        return g_gfx[Id].u.sprite;
    }
}

const ASprite* u_gfx_getNext(UGfxId Id)
{
    if(g_gfx[Id].frames) {
        return a_spriteframes_getNext(g_gfx[Id].u.frames);
    } else {
        return g_gfx[Id].u.sprite;
    }
}
