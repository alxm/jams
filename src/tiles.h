/*
    Copyright 2016 Alex Margarit

    This file is part of Motel 37.

    Motel 37 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Motel 37 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Motel 37.  If not, see <http://www.gnu.org/licenses/>.
*/

#define Z_TILE_DIM 16

typedef enum ZTileType {
    Z_TILE_TYPE_GROUND, // 0
    Z_TILE_TYPE_PEBBLES, // 1
    Z_TILE_TYPE_ROCK, // 2
    Z_TILE_TYPE_PEBBLES2, // 3
    Z_TILE_TYPE_ROCK2, // 4
    Z_TILE_TYPE_WEEDS, // 5
    Z_TILE_TYPE_PEBBLES3, // 6
    Z_TILE_TYPE_FENCE, // 7
    Z_TILE_TYPE_WALL_LEFT, // 8
    Z_TILE_TYPE_WALL_RIGHT, // 9
    Z_TILE_TYPE_WALL_BOTTOM, // a
    Z_TILE_TYPE_WALL_WINDOW, // b
    Z_TILE_TYPE_WALL, // c
    Z_TILE_TYPE_DOOR, // d
    Z_TILE_TYPE_FLOOR, // e
    Z_TILE_TYPE_NUM
} ZTileType;

extern void z_tiles_load(void);

extern ASprite* z_tiles_sprite(size_t Index);
extern bool z_tiles_isWalkable(size_t Index);
