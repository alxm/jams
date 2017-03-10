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
    z_graphic_new("goodbad", sh, 0, 21, 1);

    z_graphic_new("playerShipUp", sh, 0, 42, 1);
    z_graphic_new("playerShipDown", sh, 0, 63, 1);
    z_graphic_new("playerShipLeft", sh, 0, 84, 1);
    z_graphic_new("playerShipRight", sh, 0, 105, 1);

    z_graphic_new("satellite1", sh, 0, 126, 1);

    z_graphic_new("ship1Up", sh, 0, 147, 1);
    z_graphic_new("ship1Down", sh, 0, 168, 1);
    z_graphic_new("ship1Left", sh, 0, 189, 1);
    z_graphic_new("ship1Right", sh, 0, 210, 1);

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

void z_graphics_drawBar(uint32_t Hexcode1, uint32_t Hexcode2, int Value, int OutOf, int X, int Y, int Width, int Height)
{
    int part1Width = Width * Value / OutOf;
    int part2Width = Width - part1Width;

    a_pixel_setHex(Hexcode1);
    a_draw_rectangle(X, Y, part1Width, Height);

    a_pixel_setHex(Hexcode2);
    a_draw_rectangle(X + part1Width, Y, part2Width, Height);
}
