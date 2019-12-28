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

typedef enum {
    Z_HELP_NONE,
    Z_HELP_TITLESCREEN,
    Z_HELP_MAINGAME,
    Z_HELP_INVESTMENT,
    Z_HELP_GAMEOVER,
} ZHelpScreen;

typedef struct ZGraphic ZGraphic;

typedef struct ZColors {
    APixel darkGray;
    APixel mediumGray;
    APixel lightGray;
    APixel cyan;
    APixel magenta;
} ZColors;

typedef struct ZFont {
    unsigned darkGray;
    unsigned mediumGray;
    unsigned lightGray;
    unsigned cyan;
    unsigned magenta;
} ZFont;

extern ZColors z_colors;
extern ZFont z_fonts;

extern void z_graphics_load(void);
extern void z_graphics_unload(void);

extern const ZGraphic* z_graphics_get(const char* Name);

extern unsigned z_graphics_numFrames(const ZGraphic* Graphic);
extern ASprite* z_graphics_getFrame(const ZGraphic* Graphic, unsigned Frame);
extern ASprite* z_graphics_getNextFrame(const ZGraphic* Graphic);

extern void z_graphics_drawBar(APixel Color1, APixel Color2, int Value, int OutOf, int X, int Y, int Width, int Height);
extern void z_graphics_drawHelp(ZHelpScreen Screen);
