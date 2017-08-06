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

#include "state_game.h"

#include "util_tiles.h"

#include "component_item.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_volume.h"

#include "entity_item.h"

AEntity* z_entity_item_new(ZStateGame* Game, ZEntityItemType Type, int TileX, int TileY)
{
    const char* name;
    const char* spriteId;
    int volumeRadius;

    switch(Type) {
        case Z_ENTITY_ITEM_COFFER: {
            name = "Treasure";
            spriteId = "coffer";
            volumeRadius = 4;
        } break;

        default: {
            name = NULL;
            spriteId = NULL;
            volumeRadius = 0;
        } break;
    }

    AEntity* e = a_entity_new("item", Game);

    AFix x = a_fix_itofix(TileX * Z_UTIL_TILE_DIM + Z_UTIL_TILE_DIM / 2);
    AFix y = a_fix_itofix(TileY * Z_UTIL_TILE_DIM + Z_UTIL_TILE_DIM / 2);

    ZCompItem* item = a_entity_addComponent(e, "item");
    z_comp_item_init(item, name);

    ZCompPosition* position = a_entity_addComponent(e, "position");
    z_comp_position_init(position, x, y);

    ZCompSprite* sprite = a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite, spriteId, spriteId, spriteId, spriteId);

    ZCompVolume* volume = a_entity_addComponent(e, "volume");
    z_comp_volume_init(volume,
                       z_state_game_getVolumeColMap(Game),
                       x,
                       y,
                       volumeRadius);

    return e;
}
