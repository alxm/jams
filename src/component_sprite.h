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

typedef struct ZCompSprite ZCompSprite;

typedef enum ZCompSpriteDirection {
    Z_COMP_SPRITE_DIRECTION_UP,
    Z_COMP_SPRITE_DIRECTION_DOWN,
    Z_COMP_SPRITE_DIRECTION_LEFT,
    Z_COMP_SPRITE_DIRECTION_RIGHT,
    Z_COMP_SPRITE_DIRECTION_NUM
} ZCompSpriteDirection;

extern size_t z_comp_sprite_size(void);
extern AComponentFree z_comp_sprite_free;
extern void z_comp_sprite_init(ZCompSprite* Sprite, const char* Keys);

extern void z_comp_sprite_stop(ZCompSprite* Sprite);
extern void z_comp_sprite_move(ZCompSprite* Sprite);
extern ZCompSpriteDirection z_comp_sprite_getDir(const ZCompSprite* Sprite);
extern void z_comp_sprite_setDir(ZCompSprite* Sprite, ZCompSpriteDirection Direction);
extern ASprite* z_comp_sprite_getFrame(const ZCompSprite* Sprite);
