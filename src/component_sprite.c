/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_frames.h"

#include "component_sprite.h"

struct ZCompSprite {
    ZCompSpriteDirection direction;
    ASpriteFrames* frames[Z_COMP_SPRITE_DIR_NUM];
};

size_t z_comp_sprite_size(void)
{
    return sizeof(ZCompSprite);
}

void z_comp_sprite_init(ZCompSprite* Sprite, const char* Up, const char* Down, const char* Left, const char* Right)
{
    if(Down == NULL) {
        Down = Up;
    }

    if(Left == NULL) {
        Left = Up;
    }

    if(Right == NULL) {
        Right = Up;
    }

    Sprite->direction = a_random_int(Z_COMP_SPRITE_DIR_NUM);

    Sprite->frames[Z_COMP_SPRITE_DIR_UP] = z_util_frames_dup(Up);
    Sprite->frames[Z_COMP_SPRITE_DIR_DOWN] = z_util_frames_dup(Down);
    Sprite->frames[Z_COMP_SPRITE_DIR_LEFT] = z_util_frames_dup(Left);
    Sprite->frames[Z_COMP_SPRITE_DIR_RIGHT] = z_util_frames_dup(Right);
}

void z_comp_sprite_free(void* Self)
{
    ZCompSprite* sprite = Self;

    for(ZCompSpriteDirection d = Z_COMP_SPRITE_DIR_NUM; d--; ) {
        a_spriteframes_free(sprite->frames[d], false);
    }
}

void z_comp_sprite_setDirection(ZCompSprite* Sprite, ZCompSpriteDirection Direction)
{
    Sprite->direction = Direction;
}

void z_comp_sprite_tickFrame(const ZCompSprite* Sprite)
{
    for(ZCompSpriteDirection d = Z_COMP_SPRITE_DIR_NUM; d--; ) {
        a_spriteframes_next(Sprite->frames[d]);
    }
}

ASprite* z_comp_sprite_getSprite(const ZCompSprite* Sprite)
{
    return a_spriteframes_getCurrent(Sprite->frames[Sprite->direction]);
}
