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

#include "component_map.h"

#include "system_map.h"

A_STATE(game)
{
    A_STATE_INIT
    {
        a_component_declare("map", z_comp_map_size(), NULL);

        a_system_declare("mapDraw", "map", z_system_mapDraw, NULL, false);

        a_system_draw("mapDraw");

        AEntity* map = a_entity_new();
        z_comp_map_init(a_entity_addComponent(map, "map"));
    }

    A_STATE_BODY
    {
        A_STATE_LOOP;
    }
}
