/*
    Copyright 2020 Alex Margarit <alex@alxm.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <faur.h>

typedef enum {
    U_SFX_INVALID = -1,
    U_SFX_JINGLE,
    U_SFX_HISS,
    U_SFX_NUM
} USfxId;

extern void u_sound_init(void);
extern void u_sound_uninit(void);

extern void u_sound_tick(void);

extern void u_sound_play(USfxId Sfx);
