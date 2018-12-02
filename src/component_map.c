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

#include "component_map.h"

struct CMap {
    const UMap* map;
};

static void c_map_free(void* Self)
{
    CMap* map = Self;

    A_UNUSED(map);
}

void c_map_register(int Index)
{
    a_component_new(Index, "map", sizeof(CMap), NULL, c_map_free);
}

void c_map_init(CMap* Map, UMapId Id)
{
    Map->map = u_map_get(Id);
}

const UMap* c_map_mapGet(const CMap* Map)
{
    return Map->map;
}
