/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_graphics.h"

#include "component_sprite.h"

struct ZCompSprite {
    const ZGraphic* graphics[Z_SPRITE_DIRECTION_NUM];
    ZSpriteDirection direction;
};

size_t z_comp_sprite_size(void)
{
    return sizeof(ZCompSprite);
}

void z_comp_sprite_init(ZCompSprite* Sprite, const char* Up, const char* Down, const char* Left, const char* Right)
{
    Sprite->graphics[Z_SPRITE_DIRECTION_UP] = z_graphics_get(Up);
    Sprite->graphics[Z_SPRITE_DIRECTION_DOWN] = z_graphics_get(Down);
    Sprite->graphics[Z_SPRITE_DIRECTION_LEFT] = z_graphics_get(Left);
    Sprite->graphics[Z_SPRITE_DIRECTION_RIGHT] = z_graphics_get(Right);

    Sprite->direction = Z_SPRITE_DIRECTION_UP;
}

ASprite* z_comp_sprite_getFrame(const ZCompSprite* Sprite)
{
    return z_graphics_getFrame(Sprite->graphics[Sprite->direction], 0);
}

ASprite* z_comp_sprite_getFrameFacing(const ZCompSprite* Sprite, ZSpriteDirection Direction)
{
    return z_graphics_getFrame(Sprite->graphics[Direction], 0);
}

void z_comp_sprite_setDirection(ZCompSprite* Sprite, ZSpriteDirection Direction)
{
    Sprite->direction = Direction;
}
