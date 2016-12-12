/*
    Copyright 2016 Alex Margarit

    This file is part of Motel 37.

    Motel 37 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Motel 37 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Motel 37.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "a2x_gen/gfx.h"

static AStrHash* g_stills;
static AStrHash* g_animations;

static void z_graphics_addStill(const char* Key, const ASprite* Sheet, int X, int Y)
{
    a_strhash_add(g_stills,
                  Key,
                  a_sprite_fromSprite(Sheet, X, Y));
}

static void z_graphics_addAnimation(const char* Key, const ASprite* Sheet, int X, int Y, unsigned int Speed)
{
    a_strhash_add(g_animations,
                  Key,
                  a_spriteframes_new(Sheet, X, Y, Speed));
}

void z_graphics_load(void)
{
    g_stills = a_strhash_new();
    g_animations = a_strhash_new();

    ASprite* sheet = a_sprite_fromData(gfx_tiles);

    z_graphics_addStill("playerUp", sheet, 0, 17);
    z_graphics_addStill("playerDown", sheet, 0, 34);
    z_graphics_addStill("playerLeft", sheet, 0, 51);
    z_graphics_addStill("playerRight", sheet, 0, 68);
    z_graphics_addAnimation("playerUp", sheet, 17, 17, 8);
    z_graphics_addAnimation("playerDown", sheet, 17, 34, 8);
    z_graphics_addAnimation("playerLeft", sheet, 17, 51, 8);
    z_graphics_addAnimation("playerRight", sheet, 17, 68, 8);
    z_graphics_addAnimation("tiles", sheet, 0, 0, 1);
    z_graphics_addStill("motel", sheet, 192, 32);
    z_graphics_addStill("gate", sheet, 224, 224);
    z_graphics_addStill("grave", sheet, 272, 232);
    z_graphics_addStill("markedGrave", sheet, 272, 272);
    z_graphics_addStill("nightstand1", sheet, 304, 224);
    z_graphics_addAnimation("nightstand1", sheet, 304, 272, 10);
    z_graphics_addStill("nightstand2", sheet, 321, 224);
    z_graphics_addStill("bed", sheet, 338, 224);
    z_graphics_addStill("frame", sheet, 371, 224);
    z_graphics_addStill("colors", sheet, 371, 239);
    z_graphics_addStill("stone", sheet, 192, 112);
    z_graphics_addAnimation("motelSign", sheet, 224, 144, 30);

    a_sprite_free(sheet);
}

void z_graphics_free(void)
{
    A_STRHASH_ITERATE(g_animations, ASpriteFrames*, frames) {
        a_spriteframes_free(frames, false);
    }

    a_strhash_free(g_animations);
    a_strhash_free(g_stills);
}

ASprite* z_graphics_getStill(const char* Key)
{
    return a_strhash_get(g_stills, Key);
}

ASpriteFrames* z_graphics_getAnimation(const char* Key)
{
    return a_strhash_get(g_animations, Key);
}
