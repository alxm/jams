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

#include "system_input.h"

#include "component_input.h"

#include "util_ecs.h"

static void s_input(AEntity* Entity)
{
    CInput* input = a_entity_componentReq(Entity, U_COM_INPUT);

    c_input_tick(input);
}

void s_input_register(int Index)
{
    a_system_new(Index, s_input, NULL, false);
    a_system_add(Index, U_COM_INPUT);
}
