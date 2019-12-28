/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of City, a video game.

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
    U_COM_INVALID = -1,
    U_COM_ACTION,
    U_COM_POSITION,
    U_COM_SPRITE,
    U_COM_NUM
} UComponentId;

typedef enum {
    U_SYS_INVALD = -1,
    U_SYS_MOVE_TICK,
    U_SYS_SPRITE_TICK,
    U_SYS_SPRITE_DRAW,
    U_SYS_NUM
} USystemId;

extern void u_ecs_load(void);
