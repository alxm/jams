/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_cargo.h"
#include "component_map.h"

#include "entity_macros.h"

AEntity* z_entity_asteroid_new(ZCompMap* Map)
{
    AEntity* e = z_entity_macro_spawn(Map,
                                      "Asteroid",
                                      "asteroid",
                                      "asteroid",
                                      "asteroid",
                                      "asteroid");

    ZCompCargo* cargo = a_entity_addComponent(e, "cargo");
    z_comp_cargo_add(cargo, Z_CARGO_TYPE_MINERALS, 1 + a_random_getInt(4));

    return e;
}
