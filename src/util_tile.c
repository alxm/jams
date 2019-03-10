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
    AList* instances;
    unsigned repeatTotal;
} ZTile;

struct UTileInstance {
    const ZTile* template;
    ASprite* sprite;
    unsigned repeat;
};

static ZTile g_tiles[U_TILE_ID_NUM];
static AStrHash* g_enumMap;

static inline UTileId u_tile_stringToId(const char* Name)
{
    return (UTileId)(intptr_t)a_strhash_get(g_enumMap, Name);
}

static UTileInstance* instanceNew(const ZTile* Template, const ASprite* Sheet, AVectorInt Coords, unsigned Repeat)
{
    UTileInstance* i = a_mem_malloc(sizeof(UTileInstance));

    i->template = Template;
    i->sprite = a_sprite_newFromSpriteEx(Sheet,
                                         Coords.x,
                                         Coords.y,
                                         Z_COORDS_PIXELS_PER_UNIT,
                                         Z_COORDS_PIXELS_PER_UNIT);
    i->repeat = Repeat;

    return i;
}

static void instanceFree(UTileInstance* Tile)
{
    a_sprite_free(Tile->sprite);

    free(Tile);
}

static void tileLoad(const char* Id, const ABlock* Block, const ASprite* Sheet, int OffsetX, int OffsetY)
{
    UTileId id = u_tile_stringToId(Id);

    if(id < 0 || id >= U_TILE_ID_NUM) {
        A_FATAL("Invalid enum %s (%d)", Id, id);
    }

    a_out_printf("Loading %s (%d)", Id, id);

    ZTile* t = &g_tiles[id];

    const ABlock* flags = a_block_keyGetBlock(Block, "flags");
    const ABlock* frames = a_block_keyGetBlock(Block, "frames");

    if(flags) {
        A_LIST_ITERATE(a_block_blocksGet(flags), const ABlock*, flag) {
            const char* fStr = a_block_lineGetString(flag, 0);

            if(a_str_equal(fStr, "nomove")) {
                A_FLAG_SET(t->flags, U_TILE_FLAG_NOMOVE);
            }
        }
    }

    t->color = a_block_keyGetPixel(Block, "hex");
    t->instances = a_list_new();
    t->repeatTotal = 0;

    A_LIST_ITERATE(a_block_blocksGet(frames), const ABlock*, coordsLine) {
        AVectorInt coords = {0, 0};
        unsigned repeat = 1;
        const char* line = a_block_lineGetString(coordsLine, 0);

        if(sscanf(line, "%d, %d x%u", &coords.x, &coords.y, &repeat) != 3) {
            if(sscanf(line, "%d, %d", &coords.x, &coords.y) != 2) {
                A_FATAL("Bad coords line '%s'", line);
            }
        }

        coords.x += OffsetX;
        coords.y += OffsetY;

        UTileInstance* instance = instanceNew(t, Sheet, coords, repeat);

        a_list_addLast(t->instances, instance);
        t->repeatTotal += repeat;
    }
}

static void tileFree(UTileId Tile)
{
    ZTile* t = &g_tiles[Tile];

    a_list_freeEx(t->instances, (AFree*)instanceFree);
}

void u_tile_load(void)
{
    g_enumMap = a_strhash_new();

    #define Z__X(T) a_strhash_add(g_enumMap, #T, (void*)(intptr_t)T);
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
            const char* id = a_block_lineGetString(b, 0);

            if(a_strhash_contains(g_enumMap, id)) {
                tileLoad(id, b, sheet, 0, 0);
            } else if(a_str_equal(id, "3x3")) {
                AList* tileBlocks = a_block_blocksGet(b);

                A_LIST_ITERATE(tileBlocks, const ABlock*, b) {
                    const char* name = a_block_lineGetString(b, 0);

                    a_out_printf("Loading %s*", name);

                    for(int y = 0; y < 3; y++) {
                        for(int x = 0; x < 3; x++) {
                            tileLoad(a_str_fmt512("%s%d", name, y * 3 + x + 1),
                                     b,
                                     sheet,
                                     x * Z_COORDS_PIXELS_PER_UNIT,
                                     y * Z_COORDS_PIXELS_PER_UNIT);
                        }
                    }
                }
            } else {
                a_out_error("Unknown tile %s", id);
            }
        }

        a_block_free(root);
        a_sprite_free(sheet);
        free(fullPathPrefix);
    }

    a_dir_free(dir);

}

void u_tile_unload(void)
{
    for(int t = U_TILE_ID_NUM; t--; ) {
        tileFree(t);
    }

    a_strhash_free(g_enumMap);
}

const UTileInstance* u_tile_get(UTileId Tile)
{
    unsigned num = a_list_sizeGet(g_tiles[Tile].instances);

    if(num > 0) {
        unsigned n = 0;
        unsigned target = a_random_intu(g_tiles[Tile].repeatTotal);

        A_LIST_ITERATE(g_tiles[Tile].instances, const UTileInstance*, i) {
            n += i->repeat;

            if(n > target) {
                return i;
            }
        }
    }

    A_FATAL("No instances of tile %d", Tile);
}

bool u_tile_flagsTest(UTileId Tile, UTileFlags Flags)
{
    return A_FLAG_TEST_ALL(g_tiles[Tile].flags, Flags);
}

const ASprite* u_tile_spriteGet(const UTileInstance* Tile)
{
    return Tile->sprite;
}

APixel u_tile_colorGet(UTileId Tile)
{
    return g_tiles[Tile].color;
}
