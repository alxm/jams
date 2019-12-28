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

typedef struct ZCompSprite ZCompSprite;

typedef enum {
    Z_SPRITE_DIRECTION_UP,
    Z_SPRITE_DIRECTION_DOWN,
    Z_SPRITE_DIRECTION_LEFT,
    Z_SPRITE_DIRECTION_RIGHT,
    Z_SPRITE_DIRECTION_NUM
} ZSpriteDirection;

extern size_t z_comp_sprite_size(void);
extern void z_comp_sprite_init(ZCompSprite* Sprite, const char* Up, const char* Down, const char* Left, const char* Right);

extern ASprite* z_comp_sprite_getFrame(const ZCompSprite* Sprite);
extern ASprite* z_comp_sprite_getFrameFacing(const ZCompSprite* Sprite, ZSpriteDirection Direction);
extern void z_comp_sprite_setDirection(ZCompSprite* Sprite, ZSpriteDirection Direction);
