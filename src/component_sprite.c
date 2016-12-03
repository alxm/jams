#include <a2x.h>

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
