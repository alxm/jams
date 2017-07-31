/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Despot 3900 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Despot 3900 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Despot 3900.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_gfx.h"

ZFonts z_fonts;

void z_gfx_load(void)
{
    ASprite* fontSprite = a_sprite_newFromFile("gfx/font.png");
    AFont* font = a_font_new(fontSprite, 0, 0, A_FONT_LOAD_ALL);
    a_sprite_free(fontSprite);

    z_fonts.fontGrayDark = a_font_dup(font, a_pixel_hex(0x121212));
    z_fonts.fontGrayMedium = a_font_dup(font, a_pixel_hex(0x505050));
    z_fonts.fontGrayLight = a_font_dup(font, a_pixel_hex(0xc9c9c9));
    z_fonts.fontGreenDark = a_font_dup(font, a_pixel_hex(0x18875e));
    z_fonts.fontGreenMedium = a_font_dup(font, a_pixel_hex(0x26dc9a));
    z_fonts.fontGreenLight = a_font_dup(font, a_pixel_hex(0x70ffcb));
    z_fonts.fontRedDark = a_font_dup(font, a_pixel_hex(0x5e0749));
    z_fonts.fontRedMedium = a_font_dup(font, a_pixel_hex(0xb72a94));
    z_fonts.fontRedLight = a_font_dup(font, a_pixel_hex(0xdc6cc0));
}
