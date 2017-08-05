/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

AStrHash* g_sprites; // table of ASpriteFrames

static void new(ASprite* Sheet, int X, int Y, const char* Key)
{
    a_strhash_add(g_sprites, Key, a_spriteframes_new(Sheet, X, Y, 1));
}

void z_util_sprites_load(void)
{
    g_sprites = a_strhash_new();

    ASprite* sheet = a_sprite_newFromFile("gfx/sprites.png");

    new(sheet, 0, 0, "playerUp");
    new(sheet, 0, 17, "playerDown");
    new(sheet, 0, 34, "playerLeft");
    new(sheet, 0, 51, "playerRight");
}

void z_util_sprites_free(void)
{
    a_strhash_free(g_sprites);
}

ASpriteFrames* z_util_sprites_get(const char* Key)
{
    return a_strhash_get(g_sprites, Key);
}
