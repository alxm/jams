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

#include <a2x.h>

#include "state_game.h"

#include "component_position.h"
#include "component_sprite.h"

AEntity* z_entity_worker_new(ZStateGame* Game, int X, int Y)
{
    AEntity* e = a_entity_new("worker", Game);

    ZCompPosition* position = a_entity_addComponent(e, "position");
    z_comp_position_init(position, a_fix_itofix(X), a_fix_itofix(Y));

    ZCompSprite* sprite =  a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite,
                       "workerUp",
                       "workerDown",
                       "workerLeft",
                       "workerRight");

    return e;
}
