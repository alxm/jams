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

#include "component_position.h"
#include "component_sprite.h"
#include "component_ttl.h"

AEntity* z_entity_bubble_new(ZStateGame* Game, AFix X, AFix Y, const char* Sprite)
{
    AEntity* e = a_entity_new("poof", Game);

    ZCompPosition* position = a_entity_addComponent(e, "position");
    z_comp_position_init(position, X, Y);

    ZCompSprite* sprite = a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite,
                       Sprite,
                       NULL,
                       NULL,
                       NULL,
                       Z_COMP_SPRITE_LAYER_3,
                       true);

    ZCompTtl* ttl = a_entity_addComponent(e, "ttl");
    z_comp_ttl_init(ttl, 400, NULL);

    return e;
}
