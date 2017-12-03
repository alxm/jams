/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef enum {
    Z_UTIL_TERRAIN_INVALID = -1,
    Z_UTIL_TERRAIN_PLAIN,
    Z_UTIL_TERRAIN_BUMPS,
    Z_UTIL_TERRAIN_BUBBLES,
    Z_UTIL_TERRAIN_NUM
} ZUtilTerrainType;

typedef struct ZUtilTerrainInstance ZUtilTerrainInstance;

extern void z_util_terrain_load(void);
extern void z_util_terrain_unload(void);

extern bool z_util_terrain_isWalkable(ZUtilTerrainType Type);
extern ASpriteFrames* z_util_terrain_dupFrames(ZUtilTerrainType Type);
extern APixel z_util_terrain_getColor(ZUtilTerrainType Type);
