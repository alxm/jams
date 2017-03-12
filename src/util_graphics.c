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

ZFont z_fonts;
static APixel g_colorKey;
static AStrHash* g_gfx;

static void loadGfx(const char* Key, const ASprite* Sheet, int X, int Y, unsigned Speed)
{
    ZGraphic* g = a_mem_malloc(sizeof(ZGraphic));

    g->frames = a_spriteframes_new(Sheet, X, Y, Speed);

    a_strhash_add(g_gfx, Key, g);
}

static void loadDir(const char* KeyPrefix, const ASprite* Sheet, int X, int Y, unsigned Speed)
{
    const char* directions[4] = {"Up", "Down", "Left", "Right"};
    ASprite* probeSprite = a_sprite_fromSprite(Sheet, X, Y);
    int height = a_sprite_height(probeSprite) + 1;
    a_sprite_free(probeSprite);

    for(int i = 0; i < 4; i++) {
        char buffer[64];
        sprintf(buffer, "%s%s", KeyPrefix, directions[i]);
        loadGfx(buffer, Sheet, X, Y, Speed);
        Y += height;
    }
}

void z_graphics_load(void)
{
    g_colorKey = a_sprite_getColorKey();
    g_gfx = a_strhash_new();

    ASprite* sh = a_sprite_fromFile("./gfx/sprites.png");

    loadGfx("spaceTiles", sh, 0, 0, 1);
    loadGfx("goodbad", sh, 0, 21, 1);

    loadDir("playerShip", sh, 0, 42, 1);
    loadGfx("satellite1", sh, 0, 126, 1);
    loadDir("ship1", sh, 0, 147, 1);
    loadDir("ship2", sh, 0, 231, 1);
    loadGfx("asteroid", sh, 0, 315, 1);
    loadDir("tradingShip", sh, 0, 336, 1);

    a_sprite_free(sh);

    z_fonts.lightOrange = a_font_copy(A_FONT_FACE_WHITE, a_pixel_hex(0xff9422));
    z_fonts.lightBlue = a_font_copy(A_FONT_FACE_WHITE, a_pixel_hex(0x5ac3f9));
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
