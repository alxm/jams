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

#include "util_tile.h"

#include "util_coords.h"

typedef struct {
    AVectorInt coords;
    UTileFlags flags;
    ASprite* sprite;
    uint32_t hex;
} ZTile;

static ZTile g_tiles[U_TILE_ID_NUM] = {
    [U_TILE_ID_VOID] = {{0, 0}, U_TILE_FLAG_NOMOVE, NULL, 0xaa8844},
    [U_TILE_ID_BUILDING] = {{16, 0}, U_TILE_FLAG_NOMOVE, NULL, 0x88cc28},
    [U_TILE_ID_ROAD_H] = {{32, 0}, 0, NULL, 0x44770f},
    [U_TILE_ID_ROAD_V] = {{48, 0}, 0, NULL, 0x77440f},
};

void u_tile_load(void)
{
    ASprite* sheet = a_sprite_newFromPng("assets/gfx/tiles.png");

    for(int t = 0; t < U_TILE_ID_NUM; t++) {
        g_tiles[t].sprite = a_sprite_newFromSpriteEx(sheet,
                                                     g_tiles[t].coords.x,
                                                     g_tiles[t].coords.y,
                                                     Z_COORDS_PIXELS_PER_UNIT,
                                                     Z_COORDS_PIXELS_PER_UNIT);
    }

    a_sprite_free(sheet);
}

void u_tile_unload(void)
{
    for(int t = 0; t < U_TILE_ID_NUM; t++) {
        a_sprite_free(g_tiles[t].sprite);
    }
}

bool u_tile_flagsTest(UTile Tile, UTileFlags Flags)
{
    return A_FLAG_TEST_ALL(g_tiles[Tile].flags, Flags);
}

const ASprite* u_tile_spriteGet(UTile Tile)
{
    return g_tiles[Tile].sprite;
}

APixel u_tile_colorGet(UTile Tile)
{
    return a_pixel_fromHex(g_tiles[Tile].hex);
}
