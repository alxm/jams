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
    U_GFX_INVALID = -1,
    U_GFX_FONT_DEFAULT,
    U_GFX_ICON_MSG,
    U_GFX_NOISE,
    U_GFX_SCREEN,
    U_GFX_NUM
} UGfxId;

extern void u_gfx_load(void);
extern void u_gfx_unload(void);

extern const ASprite* u_gfx_get(UGfxId Id);
extern const ASprite* u_gfx_getNext(UGfxId Id);
