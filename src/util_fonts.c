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

#include "util_colors.h"
#include "util_fonts.h"

ZUtilFonts z_util_fonts;

void z_util_fonts_load(void)
{
    ASprite* fontSprite = a_sprite_newFromFile("gfx/font.png");
    AFont* font = a_font_new(fontSprite, 0, 0, A_FONT_LOAD_ALL);
    a_sprite_free(fontSprite);

    z_util_fonts.gray = a_font_dup(font, z_util_colors.gray4);
    z_util_fonts.blue = a_font_dup(font, z_util_colors.blue2);
    z_util_fonts.red = a_font_dup(font, z_util_colors.red2);
}
