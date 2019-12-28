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

#pragma once

#include <a2x.h>

typedef enum {
    C_SPRITE_STATE_INVALID = -1,
    C_SPRITE_STATE_WALK,
    C_SPRITE_STATE_NUM
} CSpriteState;

typedef enum {
    C_SPRITE_DIR_INVALID = -1,
    C_SPRITE_DIR_UP,
    C_SPRITE_DIR_DOWN,
    C_SPRITE_DIR_LEFT,
    C_SPRITE_DIR_RIGHT,
    C_SPRITE_DIR_NUM
} CSpriteDirection;

typedef struct CSprite CSprite;

extern const size_t c_sprite_size, c_sprite_dataSize;
extern AComponentDataInit c_sprite_dataInit;
extern AFree c_sprite_dataFree;
extern AInitWithData c_sprite_initWithData;
extern AFree c_sprite_free;

extern void c_sprite_tick(CSprite* Sprite);
extern void c_sprite_draw(const CSprite* Sprite, AVectorInt Coords);

extern void c_sprite_directionSet(CSprite* Sprite, CSpriteDirection Direction);
