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

#include <a2x.h>

#include "graphics.h"
#include "tiles.h"

typedef struct ZTile {
    bool walkable;
    ASprite* sprite;
} ZTile;

static ZTile g_tiles[Z_TILE_TYPE_NUM];

void z_tiles_load(void)
{
    ASpriteFrames* frames = z_graphics_getAnimation("tiles");

    for(size_t i = 0; i < a_spriteframes_num(frames); i++) {
        g_tiles[i].sprite = a_spriteframes_getByIndex(frames, i);
    }

    g_tiles[Z_TILE_TYPE_GROUND].walkable = true;
    g_tiles[Z_TILE_TYPE_PEBBLES].walkable = true;
    g_tiles[Z_TILE_TYPE_ROCK].walkable = true;
    g_tiles[Z_TILE_TYPE_PEBBLES2].walkable = true;
    g_tiles[Z_TILE_TYPE_PEBBLES3].walkable = true;
    g_tiles[Z_TILE_TYPE_DOOR].walkable = true;
    g_tiles[Z_TILE_TYPE_FLOOR].walkable = true;
}

ASprite* z_tiles_sprite(size_t Index)
{
    return g_tiles[Index].sprite;
}

bool z_tiles_isWalkable(size_t Index)
{
    return g_tiles[Index].walkable;
}
