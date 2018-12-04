/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Cave Shrine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cave Shrine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cave Shrine.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <a2x.h>

typedef struct UMap UMap;
typedef struct UTile UTile;

typedef enum {
    U_MAP_ID_INVALID = -1,
    U_MAP_ID_CAVE,
    U_MAP_ID_FOREST,
    U_MAP_ID_NUM
} UMapId;

#define U_TILE_DIM 16

extern void u_map_load(void);
extern void u_map_unload(void);

extern const UMap* u_map_get(UMapId Id);

extern AVectorInt u_map_getDim(const UMap* Map);
extern const UTile* u_map_getTile(const UMap* Map, int X, int Y);
extern const char* u_map_getName(const UMap* Map);
extern uint32_t u_map_getColorHex(const UMap* Map);
extern AVectorInt u_map_getStartCoords(const UMap* Map);

extern const ASprite* u_tile_getSprite(const UTile* Tile);
extern int u_tile_getCode(const UTile* Tile);
extern bool u_tile_getBlock(const UTile* Tile);
