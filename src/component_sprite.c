/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of City, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "component_sprite.h"

#include "util_coords.h"

#define Z_FRAMES_NUM 4
#define Z_FRAMES_MASK (Z_FRAMES_NUM - 1)
#define Z_FRAMES_INC (4 * A_FIX_ONE / A_CONFIG_FPS_RATE_TICK)

typedef struct {
    ASprite* frames[C_SPRITE_STATE_NUM][C_SPRITE_DIR_NUM][Z_FRAMES_NUM];
} CSpriteData;

struct CSprite {
    const CSpriteData* data;
    CSpriteState state;
    CSpriteDirection direction;
    AFixu frame;
};

const size_t c_sprite_size = sizeof(CSprite);
const size_t c_sprite_dataSize = sizeof(CSpriteData);

void c_sprite_dataInit(void* Data, const ABlock* Block)
{
    CSpriteData* d = Data;

    const ABlock* files = a_block_keyGetBlock(Block, "files");
    const ABlock* walk = a_block_keyGetBlock(files, "walk");

    if(walk) {
        const char* fileName = a_block_lineGetString(walk, 1);
        const char* path = a_str_fmt512("assets/actors/%s", fileName);

        ASpriteFrames* grid = a_spriteframes_newFromPng(
                                path,
                                Z_COORDS_PIXELS_PER_UNIT,
                                Z_COORDS_PIXELS_PER_UNIT);

        for(unsigned dir = 0; dir < C_SPRITE_DIR_NUM; dir++) {
            for(unsigned f = 0; f < Z_FRAMES_NUM; f++) {
                d->frames[C_SPRITE_STATE_WALK][dir][f] =
                    a_spriteframes_getByIndex(grid, dir * Z_FRAMES_NUM + f);
            }
        }

        a_spriteframes_free(grid, false);
    }
}

void c_sprite_dataFree(void* Self)
{
    CSpriteData* d = Self;

    for(int s = 0; s < C_SPRITE_STATE_NUM; s++) {
        for(unsigned dir = 0; dir < C_SPRITE_DIR_NUM; dir++) {
            for(unsigned f = 0; f < Z_FRAMES_NUM; f++) {
                a_sprite_free(d->frames[s][dir][f]);
            }
        }
    }
}

void c_sprite_initWithData(void* Self, const void* Data, const void* Context)
{
    A_UNUSED(Context);

    CSprite* sprite = Self;

    sprite->data = Data;
    sprite->state = C_SPRITE_STATE_WALK;
    sprite->direction = C_SPRITE_DIR_DOWN;
}

void c_sprite_free(void* Self)
{
    CSprite* sprite = Self;

    A_UNUSED(sprite);
}

void c_sprite_tick(CSprite* Sprite)
{
    Sprite->frame += Z_FRAMES_INC;
}

void c_sprite_draw(const CSprite* Sprite, AVectorInt Coords)
{
    unsigned frame = a_fixu_toInt(Sprite->frame) & Z_FRAMES_MASK;
    ASprite* sprite =
        Sprite->data->frames[Sprite->state][Sprite->direction][frame];

    a_sprite_blit(sprite,
                  Coords.x - a_sprite_sizeGetWidth(sprite) / 2,
                  Coords.y - a_sprite_sizeGetHeight(sprite) * 3 / 4);
}
