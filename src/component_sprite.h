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

typedef struct ZCompSprite ZCompSprite;

typedef enum {
    Z_COMP_SPRITE_DIR_UP,
    Z_COMP_SPRITE_DIR_DOWN,
    Z_COMP_SPRITE_DIR_LEFT,
    Z_COMP_SPRITE_DIR_RIGHT,
    Z_COMP_SPRITE_DIR_NUM
} ZCompSpriteDirection;

typedef enum {
    Z_COMP_SPRITE_LAYER_1 = -1,
    Z_COMP_SPRITE_LAYER_2,
} ZCompSpriteLayer;

extern size_t z_comp_sprite_size(void);
extern void z_comp_sprite_init(ZCompSprite* Sprite, const char* Up, const char* Down, const char* Left, const char* Right, ZCompSpriteLayer Layer, bool AutoPlay);

extern ASprite* z_comp_sprite_getGraphic(const ZCompSprite* Sprite);
extern void z_comp_sprite_setDirection(ZCompSprite* Sprite, ZCompSpriteDirection Direction);

extern void z_comp_sprite_frameReset(ZCompSprite* Sprite);
extern void z_comp_sprite_frameNext(ZCompSprite* Sprite);

extern ZCompSpriteLayer z_comp_sprite_getLayer(const ZCompSprite* Sprite);
extern bool z_comp_sprite_getAutoPlay(const ZCompSprite* Sprite);
