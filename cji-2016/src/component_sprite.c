/*
    Copyright 2016 Alex Margarit

    This file is part of Church 2, a game prototype made for the
    Cool Jams Inc 48h game jam that ran from 2016-12-02 to 2016-12-04.

    Church 2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Church 2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Church 2.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_sprite.h"
#include "graphics.h"

struct ZCompSprite {
    ASprite* still[Z_COMP_SPRITE_DIRECTION_NUM];
    ASpriteFrames* frames[Z_COMP_SPRITE_DIRECTION_NUM];
    ZCompSpriteDirection direction;
    bool moving;
};

size_t z_comp_sprite_size(void)
{
    return sizeof(ZCompSprite);
}

void z_comp_sprite_free(void* Self)
{
    ZCompSprite* sprite = Self;

    for(ZCompSpriteDirection d = 0; d < Z_COMP_SPRITE_DIRECTION_NUM; d++) {
        if(sprite->frames[d] != NULL) {
            a_spriteframes_free(sprite->frames[d], false);
        }
    }
}

void z_comp_sprite_init(ZCompSprite* Sprite, const char* Keys)
{
    AStrTok* tok = a_strtok_new(Keys, " ");
    ZCompSpriteDirection dir = Z_COMP_SPRITE_DIRECTION_UP;

    A_STRTOK_ITERATE(tok, key) {
        Sprite->still[dir] = z_graphics_getStill(key);
        ASpriteFrames* frames = z_graphics_getAnimation(key);
        Sprite->frames[dir] = frames ? a_spriteframes_clone(frames) : NULL;
        dir++;
    }

    for(ZCompSpriteDirection d = dir; d < Z_COMP_SPRITE_DIRECTION_NUM; d++) {
        Sprite->still[d] = NULL;
        Sprite->frames[d] = NULL;
    }

    a_strtok_free(tok);

    Sprite->direction = Z_COMP_SPRITE_DIRECTION_UP;
    Sprite->moving = false;
}

void z_comp_sprite_stop(ZCompSprite* Sprite)
{
    for(ZCompSpriteDirection d = 0; d < Z_COMP_SPRITE_DIRECTION_NUM; d++) {
        if(Sprite->frames[d] != NULL) {
            a_spriteframes_reset(Sprite->frames[d]);
            a_spriteframes_pause(Sprite->frames[d]);
        }
    }

    Sprite->moving = false;
}

void z_comp_sprite_move(ZCompSprite* Sprite)
{
    for(ZCompSpriteDirection d = 0; d < Z_COMP_SPRITE_DIRECTION_NUM; d++) {
        if(Sprite->frames[d] != NULL) {
            a_spriteframes_resume(Sprite->frames[d]);
        }
    }

    Sprite->moving = true;
}

ZCompSpriteDirection z_comp_sprite_getDir(const ZCompSprite* Sprite)
{
    return Sprite->direction;
}

void z_comp_sprite_setDir(ZCompSprite* Sprite, ZCompSpriteDirection Direction)
{
    Sprite->direction = Direction;
}

ASprite* z_comp_sprite_getFrame(const ZCompSprite* Sprite)
{
    if(Sprite->moving) {
        return a_spriteframes_next(Sprite->frames[Sprite->direction]);
    } else {
        return Sprite->still[Sprite->direction];
    }
}
