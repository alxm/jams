/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Cave Shrine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cave Shrine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cave Shrine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "component_sprite.h"

#include "util_map.h"

struct CSprite {
    ASpriteFrames* frames[C_POSITION_NUM];
};

static void c_sprite_free(void* Self)
{
    CSprite* sprite = Self;

    for(CPositionDirection d = C_POSITION_NUM; d--; ) {
        a_spriteframes_free(sprite->frames[d], true);
    }
}

void c_sprite_register(int Index)
{
    a_component_new(Index, "sprite", sizeof(CSprite), NULL, c_sprite_free);
}

void c_sprite_init(CSprite* Sprite, const char* Path)
{
    ASprite* sheet = a_sprite_newFromFile(Path);

    for(int d = 0; d < C_POSITION_NUM; d++) {
        Sprite->frames[d] = a_spriteframes_newFromSpriteGrid(
                                sheet, 0, d * 17, U_TILE_DIM, U_TILE_DIM, 0);
    }

    a_sprite_free(sheet);
}

const ASprite* c_sprite_framesGet(const CSprite* Sprite, CPositionDirection Direction)
{
    return a_spriteframes_getCurrent(Sprite->frames[Direction]);
}
