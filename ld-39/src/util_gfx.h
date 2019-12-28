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

typedef struct ZColors {
    APixel grayDark;
    APixel grayMedium;
    APixel grayLight;
    APixel greenDark;
    APixel greenMedium;
    APixel greenLight;
    APixel redDark;
    APixel redMedium;
    APixel redLight;
} ZColors;

typedef struct ZFonts {
    AFont* grayDark;
    AFont* grayMedium;
    AFont* grayLight;
    AFont* greenDark;
    AFont* greenMedium;
    AFont* greenLight;
    AFont* redDark;
    AFont* redMedium;
    AFont* redLight;
} ZFonts;

typedef struct ZSprites {
    ASprite* fortress;
    ASpriteFrames* fortressFlags1;
    ASpriteFrames* fortressFlags2;
    ASpriteFrames* fortressFlags3;
    ASpriteFrames* fortressFlags4;
    ASprite* iconControls;
    ASprite* iconWait;
    ASprite* iconTime;
    ASprite* iconHeart;
    ASprite* iconMoney;
    ASprite* iconAge;
    ASprite* iconHealth;
    ASprite* iconLoyalty;
    ASprite* iconRevolt;
    ASprite* iconCoup;
} ZSprites;

extern ZColors z_colors;
extern ZFonts z_fonts;
extern ZSprites z_sprites;

extern void z_gfx_load(void);

extern void z_gfx_drawBar(APixel Border, APixel Color1, APixel Color2, int Value, int OutOf, int X, int Y, int Width, int Height);
