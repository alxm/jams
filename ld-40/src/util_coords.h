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
    Z_UTIL_DIRECTION_INVALID = -1,
    Z_UTIL_DIRECTION_UP,
    Z_UTIL_DIRECTION_DOWN,
    Z_UTIL_DIRECTION_LEFT,
    Z_UTIL_DIRECTION_RIGHT,
    Z_UTIL_DIRECTION_NUM
} ZUtilDirection;

#define Z_UTIL_COORDS_TILE_SHIFT (4)
#define Z_UTIL_COORDS_TILE_DIM   (1 << Z_UTIL_COORDS_TILE_SHIFT)
#define Z_UTIL_COORDS_TILE_MASK  (Z_UTIL_COORDS_TILE_DIM - 1)

static inline int z_util_coords_fixToTileInt(AFix Coord)
{
    return a_fix_fixtoi(Coord) >> Z_UTIL_COORDS_TILE_SHIFT;
}

static inline AFix z_util_coords_fixToTileFix(AFix Coord)
{
    return Coord >> Z_UTIL_COORDS_TILE_SHIFT;
}

static inline int z_util_coords_intToTileInt(int Coord)
{
    return Coord >> Z_UTIL_COORDS_TILE_SHIFT;
}

static inline int z_util_coords_intToTileOffsetInt(int Coord)
{
    return Coord & Z_UTIL_COORDS_TILE_MASK;
}

static inline int z_util_coords_tileToInt(int TileCoord)
{
    return TileCoord * Z_UTIL_COORDS_TILE_DIM;
}

static inline int z_util_coords_tileMid(int TileCoord)
{
    return TileCoord * Z_UTIL_COORDS_TILE_DIM + Z_UTIL_COORDS_TILE_DIM / 2;
}
