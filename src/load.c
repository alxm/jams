#include <a2x.h>

#include "tiles.h"

static AStrHash* g_graphics;

A_STATE(load)
{
    A_STATE_INIT
    {
        ASprite* sheet = a_sprite_fromFile("gfx/tiles.png");
        ASpriteFrames* player = a_spriteframes_new(sheet, 0, 17, 8);
        ASpriteFrames* tiles = a_spriteframes_new(sheet, 0, 0, 1);
        a_sprite_free(sheet);

        g_graphics = a_strhash_new();
        a_strhash_add(g_graphics, "player", player);
        a_strhash_add(g_graphics, "tiles", tiles);

        z_tiles_load();

        a_state_push("world");
    }

    A_STATE_FREE
    {
        A_STRHASH_ITERATE(g_graphics, ASpriteFrames*, frames) {
            a_spriteframes_free(frames, true);
        }

        a_strhash_free(g_graphics);
    }
}

ASpriteFrames* z_graphics_get(const char* Key)
{
    return a_strhash_get(g_graphics, Key);
}
