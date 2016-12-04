#include <a2x.h>

static AStrHash* g_stills;
static AStrHash* g_animations;

static void z_graphics_addStill(const char* Key, const ASprite* Sheet, int X, int Y)
{
    a_strhash_add(g_stills,
                  Key,
                  a_sprite_fromSprite(Sheet, X, Y));
}

static void z_graphics_addAnimation(const char* Key, const ASprite* Sheet, int X, int Y, unsigned int Speed)
{
    a_strhash_add(g_animations,
                  Key,
                  a_spriteframes_new(Sheet, X, Y, Speed));
}

void z_graphics_load(void)
{
    g_stills = a_strhash_new();
    g_animations = a_strhash_new();

    ASprite* sheet = a_sprite_fromFile("gfx/tiles.png");

    z_graphics_addStill("player", sheet, 0, 17);
    z_graphics_addAnimation("player", sheet, 17, 17, 8);
    z_graphics_addAnimation("tiles", sheet, 0, 0, 1);

    a_sprite_free(sheet);
}

void z_graphics_free(void)
{
    A_STRHASH_ITERATE(g_animations, ASpriteFrames*, frames) {
        a_spriteframes_free(frames, false);
    }

    a_strhash_free(g_animations);
    a_strhash_free(g_stills);
}

ASprite* z_graphics_getStill(const char* Key)
{
    return a_strhash_get(g_stills, Key);
}

ASpriteFrames* z_graphics_getAnimation(const char* Key)
{
    return a_strhash_get(g_animations, Key);
}
