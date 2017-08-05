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

#include "component_map.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_velocity.h"

void z_system_move(AEntity* Entity)
{
    ZCompPosition* position = a_entity_requireComponent(Entity, "position");
    ZCompSprite* sprite = a_entity_requireComponent(Entity, "sprite");
    ZCompVelocity* velocity = a_entity_requireComponent(Entity, "velocity");

    AFix dx, dy;
    z_comp_velocity_getValues(velocity, &dx, &dy);

    if(dx == 0 && dy == 0) {
        z_comp_sprite_frameReset(sprite);
        return;
    }

    AFix x, y;
    z_comp_position_getCoords(position, &x, &y);

    AFix newX = x + dx;
    AFix newY = y + dy;

    z_comp_position_setCoords(position, newX, newY);
    z_comp_velocity_setValues(velocity, 0, 0);

    ZStateGame* game = a_entity_getContext(Entity);
    ZCompMap* map = a_entity_requireComponent(z_state_game_getMap(game), "map");

    int tileX = a_fix_fixtoi(newX) / Z_UTIL_TILE_DIM;
    int tileY = a_fix_fixtoi(newY) / Z_UTIL_TILE_DIM;

    if(!z_comp_map_isWalkable(map, tileX, tileY)) {
        z_comp_position_setCoords(position, x, y);
    }

    z_comp_sprite_frameNext(sprite);
}
