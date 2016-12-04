#include <a2x.h>

#include "component_sprite.h"
#include "graphics.h"

struct ZCompSprite {
    ASprite* still;
    ASpriteFrames* frames;
    bool moving;
};

size_t z_comp_sprite_size(void)
{
    return sizeof(ZCompSprite);
}

void z_comp_sprite_free(void* Self)
{
    ZCompSprite* sprite = Self;

    if(sprite->frames != NULL) {
        a_spriteframes_free(sprite->frames, false);
    }
}

void z_comp_sprite_init(ZCompSprite* Sprite, const char* Key)
{
    Sprite->still = z_graphics_getStill(Key);
    ASpriteFrames* frames = z_graphics_getAnimation(Key);

    if(frames != NULL) {
        Sprite->frames = a_spriteframes_clone(frames);
    } else {
        Sprite->frames = NULL;
    }

    Sprite->moving = false;
}

void z_comp_sprite_stop(ZCompSprite* Sprite)
{
    a_spriteframes_reset(Sprite->frames);
    a_spriteframes_pause(Sprite->frames);
    Sprite->moving = false;
}

void z_comp_sprite_move(ZCompSprite* Sprite)
{
    a_spriteframes_resume(Sprite->frames);
    Sprite->moving = true;
}

ASprite* z_comp_sprite_getFrame(const ZCompSprite* Sprite)
{
    if(Sprite->moving) {
        return a_spriteframes_next(Sprite->frames);
    } else {
        return Sprite->still;
    }
}
