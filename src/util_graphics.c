/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_graphics.h"

struct ZGraphic {
    ASpriteFrames* frames;
};

static APixel g_colorKey;
static AStrHash* g_gfx;

static void z_graphic_new(const char* Key, const ASprite* Sheet, int X, int Y, unsigned Speed)
{
    ZGraphic* g = a_mem_malloc(sizeof(ZGraphic));

    g->frames = a_spriteframes_new(Sheet, X, Y, Speed);

    a_strhash_add(g_gfx, Key, g);
}

void z_graphics_load(void)
{
    g_colorKey = a_sprite_getColorKey();
    g_gfx = a_strhash_new();

    ASprite* sh = a_sprite_fromFile("./gfx/sprites.png");

    z_graphic_new("spaceTiles", sh, 0, 0, 1);
    z_graphic_new("playerShip", sh, 0, 21, 1);
    z_graphic_new("satellite", sh, 0, 42, 1);
    z_graphic_new("goodbad", sh, 0, 63, 1);

    a_sprite_free(sh);
}

void z_graphics_unload(void)
{
    A_STRHASH_ITERATE(g_gfx, ZGraphic*, g) {
        a_spriteframes_free(g->frames, true);
        free(g);
    }

    a_strhash_free(g_gfx);
}

const ZGraphic* z_graphics_get(const char* Name)
{
    return a_strhash_get(g_gfx, Name);
}

unsigned z_graphics_numFrames(const ZGraphic* Graphic)
{
    return a_spriteframes_num(Graphic->frames);
}

ASprite* z_graphics_getFrame(const ZGraphic* Graphic, unsigned Frame)
{
    return a_spriteframes_getByIndex(Graphic->frames, Frame);
}
