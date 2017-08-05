/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_tiles.h"

struct ZUtilTile {
    ASprite* sprite;
    bool walkable;
};

static AList* g_tiles; // list of ZTile

void z_util_tiles_load(void)
{
    g_tiles = a_list_new();

    ASprite* sheet = a_sprite_newFromFile("gfx/tiles.png");
    ASpriteFrames* frames = a_spriteframes_new(sheet, 0, 1, 1);

    A_LIST_ITERATE(a_spriteframes_getSprites(frames), ASprite*, s) {
        ZUtilTile* tile = a_mem_malloc(sizeof(ZUtilTile));
        int x = (int)A_LIST_INDEX() * (Z_UTIL_TILE_DIM + 1);

        tile->sprite = s;
        tile->walkable = a_sprite_getPixel(sheet, x, 0) == 0;

        a_list_addLast(g_tiles, tile);
    }

    a_spriteframes_free(frames, false);
}

void z_util_tiles_free(void)
{
    a_list_freeEx(g_tiles, free);
}

const ZUtilTile* z_util_tiles_getTile(unsigned Index)
{
    return a_list_getIndex(g_tiles, Index);
}

ASprite* z_util_tiles_getSprite(const ZUtilTile* Tile)
{
    return Tile->sprite;

}

bool z_util_tiles_isWalkable(const ZUtilTile* Tile)
{
    return Tile->walkable;
}
