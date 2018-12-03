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

#include "util_ecs.h"

#include "component_map.h"
#include "component_position.h"
#include "component_sprite.h"

#include "system_mapdraw.h"
#include "system_spritedraw.h"

static void u_components_load(void)
{
    c_map_register(U_COM_MAP);
    c_position_register(U_COM_POSITION);
    c_sprite_register(U_COM_SPRITE);
}

static void u_systems_load(void)
{
    s_mapdraw_register(U_SYS_MAPDRAW);
    s_spritedraw_register(U_SYS_SPRITEDRAW);
}

void u_ecs_load(void)
{
    a_ecs_init(U_COM_NUM, U_SYS_NUM, U_MSG_NUM);

    u_components_load();
    u_systems_load();
}
