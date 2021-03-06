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

#include "component_map.h"

AEntity* z_entity_map_new(ZStateGame* Game, const char* DataFile)
{
    AEntity* e = a_entity_new("map", Game);
    ASprite* sprite = a_sprite_newFromFile(DataFile);

    ZCompMap* map = a_entity_addComponent(e, "map");
    z_comp_map_init(map, sprite);

    a_sprite_free(sprite);

    return e;
}
