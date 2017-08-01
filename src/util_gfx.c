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

ZColors z_colors;
ZFonts z_fonts;
ZSprites z_sprites;

static void loadColors(void)
{
    z_colors.grayDark = a_pixel_hex(0x222222);
    z_colors.grayMedium = a_pixel_hex(0x505050);
    z_colors.grayLight = a_pixel_hex(0xc9c9c9);
    z_colors.greenDark = a_pixel_hex(0x18875e);
    z_colors.greenMedium = a_pixel_hex(0x26dc9a);
    z_colors.greenLight = a_pixel_hex(0x70ffcb);
    z_colors.redDark = a_pixel_hex(0x5e0749);
    z_colors.redMedium = a_pixel_hex(0xb72a94);
    z_colors.redLight = a_pixel_hex(0xdc6cc0);
}

static void loadFonts(void)
{
    ASprite* fontSprite = a_sprite_newFromFile("gfx/font.png");
    AFont* font = a_font_new(fontSprite, 0, 0, A_FONT_LOAD_ALL);
    a_sprite_free(fontSprite);

    z_fonts.grayDark = a_font_dup(font, z_colors.grayDark);
    z_fonts.grayMedium = a_font_dup(font, z_colors.grayMedium);
    z_fonts.grayLight = a_font_dup(font, z_colors.grayLight);
    z_fonts.greenDark = a_font_dup(font, z_colors.greenDark);
    z_fonts.greenMedium = a_font_dup(font, z_colors.greenMedium);
    z_fonts.greenLight = a_font_dup(font, z_colors.greenLight);
    z_fonts.redDark = a_font_dup(font, z_colors.redDark);
    z_fonts.redMedium = a_font_dup(font, z_colors.redMedium);
    z_fonts.redLight = a_font_dup(font, z_colors.redLight);
}

static void loadSprites(void)
{
    z_sprites.fortress = a_sprite_newFromFile("gfx/fortress.png");

    ASprite* flags = a_sprite_newFromFile("gfx/fortress_flags.png");
    z_sprites.fortressFlags1 = a_spriteframes_new(flags, 0, 0, 8);
    z_sprites.fortressFlags2 = a_spriteframes_new(flags, 0, 17, 16);
    z_sprites.fortressFlags3 = a_spriteframes_new(flags, 18, 17, 16);
    z_sprites.fortressFlags4 = a_spriteframes_new(flags, 36, 17, 16);

    z_sprites.iconControls = a_sprite_newFromFile("gfx/icon_help.png");
    z_sprites.iconWait = a_sprite_newFromFile("gfx/icon_wait.png");
    z_sprites.iconTime = a_sprite_newFromFile("gfx/icon_time.png");
    z_sprites.iconHeart = a_sprite_newFromFile("gfx/icon_heart.png");
    z_sprites.iconMoney = a_sprite_newFromFile("gfx/icon_money.png");
    z_sprites.iconAge = a_sprite_newFromFile("gfx/icon_age.png");
    z_sprites.iconHealth = a_sprite_newFromFile("gfx/icon_health.png");
    z_sprites.iconLoyalty = a_sprite_newFromFile("gfx/icon_loyalty.png");
    z_sprites.iconRevolt = a_sprite_newFromFile("gfx/icon_revolt.png");
    z_sprites.iconCoup = a_sprite_newFromFile("gfx/icon_coup.png");
}

void z_gfx_load(void)
{
    loadColors();
    loadFonts();
    loadSprites();
}

void z_gfx_drawBar(APixel Border, APixel Color1, APixel Color2, int Value, int OutOf, int X, int Y, int Width, int Height)
{
    int part1Width = a_math_min(Width * Value / OutOf, Width);
    int part2Width = Width - part1Width;

    a_pixel_setPixel(Color1);
    a_draw_rectangle(X, Y, part1Width, Height);

    a_pixel_setPixel(Color2);
    a_draw_rectangle(X + part1Width, Y, part2Width, Height);

    a_pixel_setPixel(Border);
    a_draw_hline(X, X + Width, Y);
    a_draw_hline(X, X + Width, Y + Height - 1);
    a_draw_vline(X, Y, Y + Height);
    a_draw_vline(X + Width - 1, Y, Y + Height);
}
