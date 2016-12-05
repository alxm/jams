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
    g_tiles[Z_TILE_TYPE_ROCK2].walkable = false;
    g_tiles[Z_TILE_TYPE_WEEDS].walkable = false;
}

ASprite* z_tiles_sprite(size_t Index)
{
    return g_tiles[Index].sprite;
}

bool z_tiles_isWalkable(size_t Index)
{
    return g_tiles[Index].walkable;
}
