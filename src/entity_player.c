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

#include "component_position.h"
#include "component_sprite.h"

AEntity* z_entity_player_new(ZStateGame* Game, int TileX, int TileY)
{
    AEntity* e = a_entity_new("player", Game);

    ZCompPosition* position = a_entity_addComponent(e, "position");
    z_comp_position_init(position,
                         a_fix_itofix(TileX * Z_UTIL_TILE_DIM
                                        + Z_UTIL_TILE_DIM / 2),
                         a_fix_itofix(TileY * Z_UTIL_TILE_DIM
                                        + Z_UTIL_TILE_DIM / 2));

    ZCompSprite* sprite = a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite,
                       "playerUp",
                       "playerDown",
                       "playerLeft",
                       "playerRight");

    return e;
}
