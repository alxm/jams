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
    U_TILE_ID_INVALID = -1,
    U_TILE_ID_VOID,
    U_TILE_ID_BUILDING,
    U_TILE_ID_ROAD_H,
    U_TILE_ID_ROAD_V,
    U_TILE_ID_NUM
} UTile;

typedef enum {
    U_TILE_FLAG_NOMOVE = A_FLAG_BIT(0),
} UTileFlags;

extern void u_tile_load(void);
extern void u_tile_unload(void);

extern bool u_tile_flagsTest(UTile Tile, UTileFlags Flags);
extern const ASprite* u_tile_spriteGet(UTile Tile);
extern APixel u_tile_colorGet(UTile Tile);
