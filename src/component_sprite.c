#include <a2x.h>

#include "load.h"

typedef struct ZCompSprite {
    ASpriteFrames* frames;
} ZCompSprite;

size_t z_comp_sprite_size(void)
{
    return sizeof(ZCompSprite);
}

void z_comp_sprite_free(void* Self)
{
    ZCompSprite* sprite = Self;
    a_spriteframes_free(sprite->frames, false);
}

void z_comp_sprite_init(ZCompSprite* Sprite, const char* Key)
{
    Sprite->frames = a_spriteframes_clone(z_graphics_get(Key));
}

ASprite* z_comp_sprite_getFrame(const ZCompSprite* Sprite)
{
    return a_spriteframes_next(Sprite->frames);
}
