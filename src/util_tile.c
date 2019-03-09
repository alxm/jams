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
    UTileFlags flags;
    APixel color;
    ASprite* sprite;
} ZTile;

static ZTile g_tiles[U_TILE_ID_NUM];

static void tileLoad(UTileId Tile, const ABlock* Block, const ASprite* Sheet)
{
    ZTile* t = &g_tiles[Tile];

    const ABlock* flags = a_block_keyGetBlock(Block, "flags");
    AVectorInt coords = a_block_keyGetCoords(Block, "frames");

    if(flags) {
        A_LIST_ITERATE(a_block_blocksGet(flags), const ABlock*, flag) {
            const char* fStr = a_block_lineGetString(flag, 0);

            if(a_str_equal(fStr, "nomove")) {
                A_FLAG_SET(t->flags, U_TILE_FLAG_NOMOVE);
            }
        }
    }

    t->color = a_block_keyGetPixel(Block, "hex");
    t->sprite = a_sprite_newFromSpriteEx(Sheet,
                                         coords.x,
                                         coords.y,
                                         Z_COORDS_PIXELS_PER_UNIT,
                                         Z_COORDS_PIXELS_PER_UNIT);
}

void u_tile_load(void)
{
    AStrHash* map = a_strhash_new();

    #define Z__X(T) a_strhash_add(map, #T, (void*)(intptr_t)T);
    U__TILE_X
    #undef Z__X

    ADir* dir = a_dir_new("assets/tiles");

    A_LIST_ITERATE(a_dir_entriesListGet(dir), const APath*, path) {
        if(!a_str_endsWith(a_path_getName(path), ".txt")) {
            continue;
        }

        const char* fullPath = a_path_getFull(path);
        char* fullPathPrefix = a_str_prefixGetToLast(fullPath, '.');

        char buffer[64];
        a_str_fmt(buffer, sizeof(buffer), false, "%s.png", fullPathPrefix);

        ASprite* sheet = a_sprite_newFromPng(buffer);
        ABlock* root = a_block_new(a_path_getFull(path));

        A_LIST_ITERATE(a_block_blocksGet(root), const ABlock*, b) {
            tileLoad((UTileId)(intptr_t)
                        a_strhash_get(map, a_block_lineGetString(b, 0)),
                     b,
                     sheet);
        }

        a_block_free(root);
        a_sprite_free(sheet);
        free(fullPathPrefix);
    }

    a_dir_free(dir);
    a_strhash_free(map);
}

void u_tile_unload(void)
{
    for(int t = 0; t < U_TILE_ID_NUM; t++) {
        a_sprite_free(g_tiles[t].sprite);
    }
}

bool u_tile_flagsTest(UTileId Tile, UTileFlags Flags)
{
    return A_FLAG_TEST_ALL(g_tiles[Tile].flags, Flags);
}

const ASprite* u_tile_spriteGet(UTileId Tile)
{
    return g_tiles[Tile].sprite;
}

APixel u_tile_colorGet(UTileId Tile)
{
    return g_tiles[Tile].color;
}
