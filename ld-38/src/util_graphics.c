/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    38th Century - A simple world simulation game made for Ludum Dare 38 72h

    38th Century is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    38th Century is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 38th Century.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>
#include "a2x_gen/gfx.h"

#include "util_controls.h"
#include "util_graphics.h"

struct ZGraphic {
    ASpriteFrames* frames;
};

ZColors z_colors;
ZFont z_fonts;
static APixel g_colorKey;
static AStrHash* g_gfx;

static void loadGfx(const char* Key, const ASprite* Sheet, int X, int Y, unsigned FramesSpeed)
{
    ZGraphic* g = a_mem_malloc(sizeof(ZGraphic));

    g->frames = a_spriteframes_new(Sheet, X, Y, FramesSpeed);

    a_strhash_add(g_gfx, Key, g);
}

void z_graphics_load(void)
{
    g_colorKey = a_sprite_getColorKey();
    g_gfx = a_strhash_new();

    ASprite* sh = a_sprite_fromData(gfx_gfx, "gfx");

    loadGfx("iconsLight", sh, 106, 4, 1);
    loadGfx("iconsDark", sh, 106, 13, 1);
    loadGfx("clouds", sh, 319, 0, 1);
    loadGfx("cave", sh, 0, 0, 5);

    a_sprite_free(sh);

    z_colors.darkGray = a_pixel_hex(0x111111);
    z_colors.mediumGray = a_pixel_hex(0x222222);
    z_colors.lightGray = a_pixel_hex(0x888888);
    z_colors.cyan = a_pixel_hex(0x00dddd);
    z_colors.magenta = a_pixel_hex(0xdd00dd);

    z_fonts.darkGray = a_font_dup(A_FONT_FACE_WHITE, z_colors.darkGray);
    z_fonts.mediumGray = a_font_dup(A_FONT_FACE_WHITE, z_colors.mediumGray);
    z_fonts.lightGray = a_font_dup(A_FONT_FACE_WHITE, z_colors.lightGray);
    z_fonts.cyan = a_font_dup(A_FONT_FACE_WHITE, z_colors.cyan);
    z_fonts.magenta = a_font_dup(A_FONT_FACE_WHITE, z_colors.magenta);
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

ASprite* z_graphics_getNextFrame(const ZGraphic* Graphic)
{
    return a_spriteframes_next(Graphic->frames);
}

void z_graphics_drawBar(APixel Color1, APixel Color2, int Value, int OutOf, int X, int Y, int Width, int Height)
{
    int part1Width = a_math_min(Width * Value / OutOf, Width);
    int part2Width = Width - part1Width;

    a_pixel_setPixel(Color1);
    a_draw_rectangle(X, Y, part1Width, Height);

    a_pixel_setPixel(Color2);
    a_draw_rectangle(X + part1Width, Y, part2Width, Height);
}

void z_graphics_drawHelp(ZHelpScreen Screen)
{
    a_font_setFace(z_fonts.magenta);
    int y = a_screen_height() - a_font_getLineHeight() - 2;

    a_pixel_setPixel(z_colors.mediumGray);
    a_draw_rectangle(0, y, a_screen_width(), a_font_getLineHeight() + 4);

    a_pixel_setPixel(z_colors.lightGray);
    a_draw_hline(0, a_screen_width(), y - 1);

    a_font_setCoords(2, y + 2);

    switch(Screen) {
        case Z_HELP_TITLESCREEN: {
            //
        } break;

        case Z_HELP_MAINGAME: {
            a_font_setFace(z_fonts.lightGray);
            a_font_text("Press ");
            a_font_setFace(z_fonts.magenta);
            a_font_text(a_button_name(z_controls.primary));
            a_font_setFace(z_fonts.lightGray);
            a_font_text(" to pass a decade");
        } break;

        case Z_HELP_INVESTMENT: {
            a_font_setFace(z_fonts.lightGray);
            a_font_text("Use ");
            a_font_setFace(z_fonts.magenta);
            a_font_text("arrows");
            a_font_setFace(z_fonts.lightGray);
            a_font_text(" to browse, press ");
            a_font_setFace(z_fonts.magenta);
            a_font_text(a_button_name(z_controls.primary));
            a_font_setFace(z_fonts.lightGray);
            a_font_text(" to select investment");
        } break;

        case Z_HELP_GAMEOVER: {
            a_font_setFace(z_fonts.lightGray);
            a_font_text("Press ");
            a_font_setFace(z_fonts.magenta);
            a_font_text(a_button_name(z_controls.start));
            a_font_setFace(z_fonts.lightGray);
            a_font_text(" to continue");
        } break;

        default: break;
    }
}
