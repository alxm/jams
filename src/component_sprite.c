/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_sprites.h"

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
    Sprite->direction = Z_COMP_SPRITE_DIR_DOWN;

    Sprite->frames[Z_COMP_SPRITE_DIR_UP] = a_spriteframes_dup(
                                                z_util_sprites_get(Up));
    Sprite->frames[Z_COMP_SPRITE_DIR_DOWN] = a_spriteframes_dup(
                                                z_util_sprites_get(Down));
    Sprite->frames[Z_COMP_SPRITE_DIR_LEFT] = a_spriteframes_dup(
                                                z_util_sprites_get(Left));
    Sprite->frames[Z_COMP_SPRITE_DIR_RIGHT] = a_spriteframes_dup(
                                                z_util_sprites_get(Right));
}

ASprite* z_comp_sprite_getGraphic(const ZCompSprite* Sprite)
{
    return a_spriteframes_getCurrent(Sprite->frames[Sprite->direction]);
}

void z_comp_sprite_setDirection(ZCompSprite* Sprite, ZCompSpriteDirection Direction)
{
    Sprite->direction = Direction;
}

void z_comp_sprite_frameReset(ZCompSprite* Sprite)
{
    for(ZCompSpriteDirection d = Z_COMP_SPRITE_DIR_NUM; d--; ) {
        a_spriteframes_reset(Sprite->frames[d]);
    }
}

void z_comp_sprite_frameNext(ZCompSprite* Sprite)
{
    for(ZCompSpriteDirection d = Z_COMP_SPRITE_DIR_NUM; d--; ) {
        a_spriteframes_next(Sprite->frames[d]);
    }
}
