#include <a2x.h>

#include "load.h"
#include "tiles.h"

typedef struct ZTile {
    bool walkable;
    ASprite* sprite;
} ZTile;

static ZTile g_tiles[Z_TILE_TYPE_NUM];

void z_tiles_load(void)
{
    ASpriteFrames* frames = z_graphics_get("tiles");

    for(size_t i = 0; i < a_spriteframes_num(frames); i++) {
        g_tiles[i].sprite = a_spriteframes_getByIndex(frames, i);
    }

    g_tiles[Z_TILE_TYPE_GROUND].walkable = true;
    g_tiles[Z_TILE_TYPE_PEBBLES].walkable = true;
}
