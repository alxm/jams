#include <a2x.h>

static AStrHash* g_graphics;

void z_graphics_load(void)
{
    g_graphics = a_strhash_new();
    ASprite* sheet = a_sprite_fromFile("gfx/tiles.png");

    a_strhash_add(g_graphics,
                  "player",
                  a_spriteframes_new(sheet, 0, 17, 8));

    a_strhash_add(g_graphics,
                  "tiles",
                  a_spriteframes_new(sheet, 0, 0, 1));

    a_sprite_free(sheet);
}

void z_graphics_free(void)
{
    A_STRHASH_ITERATE(g_graphics, ASpriteFrames*, frames) {
        a_spriteframes_free(frames, true);
    }

    a_strhash_free(g_graphics);
}

ASpriteFrames* z_graphics_get(const char* Key)
{
    return a_strhash_get(g_graphics, Key);
}
