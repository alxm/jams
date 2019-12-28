/*
    Copyright 2016 Alex Margarit

    This file is part of Church 2, a game prototype made for the
    Cool Jams Inc 48h game jam that ran from 2016-12-02 to 2016-12-04.

    Church 2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Church 2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Church 2.  If not, see <http://www.gnu.org/licenses/>.
*/

#define Z_TILE_DIM 16

typedef enum ZTileType {
    Z_TILE_TYPE_GROUND,
    Z_TILE_TYPE_PEBBLES,
    Z_TILE_TYPE_ROCK,
    Z_TILE_TYPE_PEBBLES2,
    Z_TILE_TYPE_ROCK2,
    Z_TILE_TYPE_WEEDS,
    Z_TILE_TYPE_PEBBLES3,
    Z_TILE_TYPE_NUM
} ZTileType;

extern void z_tiles_load(void);

extern ASprite* z_tiles_sprite(size_t Index);
extern bool z_tiles_isWalkable(size_t Index);
