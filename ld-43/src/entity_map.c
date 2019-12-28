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

#include "component_map.h"

#include "util_ecs.h"

AEntity* e_map_new(TGame* Game, UMapId Id)
{
    AEntity* e = a_entity_new("map", Game);

    CMap* map = a_entity_componentAdd(e, U_COM_MAP);
    c_map_init(map, Id);

    a_entity_activeSetPermanent(e);

    return e;
}

const UMap* e_map_mapGet(const AEntity* Map)
{
    CMap* map = a_entity_componentReq(Map, U_COM_MAP);

    return c_map_mapGet(map);
}
