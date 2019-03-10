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

#define Z__3(T) \
    Z__X(T##1) \
    Z__X(T##2) \
    Z__X(T##3) \
    Z__X(T##4) \
    Z__X(T##5) \
    Z__X(T##6) \
    Z__X(T##7) \
    Z__X(T##8) \
    Z__X(T##9) \

#define U__TILE_X \
    \
    Z__X(U_TILE_ID_MISC_VOID) \
    Z__3(U_TILE_ID_MISC_TREE_) \
    \
    Z__X(U_TILE_ID_ROAD_H) \
    Z__X(U_TILE_ID_ROAD_V) \
    \
    Z__3(U_TILE_ID_SIDEWALK_) \
    \
    Z__3(U_TILE_ID_B0_ROOF_) \
    Z__3(U_TILE_ID_B0_FRONT_) \
    Z__X(U_TILE_ID_B0_DOOR_7) \
    Z__X(U_TILE_ID_B0_DOOR_8) \
    Z__X(U_TILE_ID_B0_DOOR_9) \
    \

#define Z__X(T) T,

typedef enum {
    U_TILE_ID_INVALID = -1,
    U__TILE_X
    U_TILE_ID_NUM
} UTileId;

#undef Z__X

typedef enum {
    U_TILE_FLAG_NOMOVE = A_FLAG_BIT(0),
} UTileFlags;

typedef struct UTileInstance UTileInstance;

extern void u_tile_load(void);
extern void u_tile_unload(void);

extern const UTileInstance* u_tile_get(UTileId Tile);

extern bool u_tile_flagsTest(UTileId Tile, UTileFlags Flags);
extern const ASprite* u_tile_spriteGet(const UTileInstance* Tile);
extern APixel u_tile_colorGet(UTileId Tile);
