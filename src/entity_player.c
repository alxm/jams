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

#include "entity_map.h"

#include "component_position.h"
#include "component_sprite.h"

#include "util_ecs.h"

AEntity* e_player_new(TGame* Game, int X, int Y)
{
    AEntity* e = a_entity_new("player", Game);

    CPosition* position = a_entity_componentAdd(e, U_COM_POSITION);
    c_position_coordsSet(position, (AVectorInt){X, Y});
    c_position_directionSet(position, C_POSITION_DOWN);

    CSprite* sprite = a_entity_componentAdd(e, U_COM_SPRITE);
    c_sprite_init(sprite, "assets/gfx/player.png");

    a_entity_activeSetPermanent(e);

    return e;
}
