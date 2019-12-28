/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of Coffin Digital, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <a2x.h>

typedef enum {
    U_FONT_INVALID = -1,
    U_FONT_DEFAULT,
    U_FONT_GRAY_LIGHT,
    U_FONT_GRAY_MEDIUM,
    U_FONT_PINK,
    U_FONT_PURPLE,
    U_FONT_YELLOW,
    U_FONT_NUM,
} UFontId;

extern void u_font_load(void);
extern void u_font_unload(void);

extern const AFont* u_font_get(UFontId Id);
extern void u_font_int(int Number, int NumDigits, UFontId FontPadding, UFontId FontNumber, int X, int Y);
