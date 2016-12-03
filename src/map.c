#include <a2x.h>

#include "load.h"

typedef enum ZTileType {
    Z_TILE_TYPE_GROUND,
    Z_TILE_TYPE_PEBBLES,
    Z_TILE_TYPE_NUM
} ZTileType;

typedef struct ZTile {
    bool walkable;
    ASprite* sprite;
} ZTile;

typedef struct ZMap {
    int w, h;
    int** tiles; // index to tiles
} ZMap;

static ZTile g_tiles[Z_TILE_TYPE_NUM];

void z_map_tile_load(void)
{
    ASpriteFrames* frames = z_graphics_get("tiles");

    for(size_t i = 0; i < a_spriteframes_num(frames); i++) {
        g_tiles[i].sprite = a_spriteframes_getByIndex(frames, i);
    }

    g_tiles[Z_TILE_TYPE_GROUND].walkable = true;
}
