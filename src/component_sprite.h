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

typedef struct ZCompSprite ZCompSprite;

typedef enum {
    Z_COMP_SPRITE_LAYER_INVALID = -1,
    Z_COMP_SPRITE_LAYER_BUILDINGS,
    Z_COMP_SPRITE_LAYER_UNITS,
    Z_COMP_SPRITE_LAYER_CURSOR,
} ZCompSpriteLayer;

typedef enum {
    Z_COMP_SPRITE_DIR_UP,
    Z_COMP_SPRITE_DIR_DOWN,
    Z_COMP_SPRITE_DIR_LEFT,
    Z_COMP_SPRITE_DIR_RIGHT,
    Z_COMP_SPRITE_DIR_NUM
} ZCompSpriteDirection;

extern size_t z_comp_sprite_size(void);
extern void z_comp_sprite_init(ZCompSprite* Sprite, const char* Id, ZCompSpriteLayer Layer);
extern void z_comp_sprite_initEx(ZCompSprite* Sprite, const char* Up, const char* Down, const char* Left, const char* Right, ZCompSpriteLayer Layer);
extern AFree z_comp_sprite_free;

extern void z_comp_sprite_setDirection(ZCompSprite* Sprite, ZCompSpriteDirection Direction);
extern void z_comp_sprite_tickFrame(const ZCompSprite* Sprite);
extern ASprite* z_comp_sprite_getSprite(const ZCompSprite* Sprite);
extern ZCompSpriteLayer z_comp_sprite_getLayer(const ZCompSprite* Sprite);
