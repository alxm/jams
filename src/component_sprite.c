#include <a2x.h>

#include "component_sprite.h"
#include "graphics.h"

struct ZCompSprite {
    ASpriteFrames* frames;
};

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

void z_comp_sprite_stop(const ZCompSprite* Sprite)
{
    a_spriteframes_reset(Sprite->frames);
    a_spriteframes_pause(Sprite->frames);
}

void z_comp_sprite_move(const ZCompSprite* Sprite)
{
    a_spriteframes_resume(Sprite->frames);
}

ASprite* z_comp_sprite_getFrame(const ZCompSprite* Sprite)
{
    return a_spriteframes_next(Sprite->frames);
}
