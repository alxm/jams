/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of City, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "util_ecs.h"

#include "component_action.h"
#include "component_position.h"
#include "component_sprite.h"

#include "system_move.h"
#include "system_sprite.h"

void u_ecs_load(void)
{
    a_ecs_init(U_COM_NUM, U_SYS_NUM);

    a_component_new(U_COM_ACTION, "action", c_action_size, c_action_init, c_action_free);
    a_component_new(U_COM_POSITION, "position", c_position_size, NULL, c_position_free);
    a_component_newEx(U_COM_SPRITE, "sprite", c_sprite_size, c_sprite_initWithData, c_sprite_free, c_sprite_dataSize, c_sprite_dataInit, c_sprite_dataFree);

    a_system_new(U_SYS_MOVE_TICK, s_move_tick, NULL, true);

    a_system_new(U_SYS_SPRITE_TICK, s_sprite_tick, NULL, false);
    a_system_add(U_SYS_SPRITE_TICK, U_COM_POSITION);
    a_system_add(U_SYS_SPRITE_TICK, U_COM_SPRITE);

    a_system_new(U_SYS_SPRITE_DRAW, s_sprite_draw, NULL, false);
    a_system_add(U_SYS_SPRITE_DRAW, U_COM_POSITION);
    a_system_add(U_SYS_SPRITE_DRAW, U_COM_SPRITE);

    ADir* dir = a_dir_new("assets/actors");

    A_LIST_ITERATE(a_dir_entriesListGet(dir), APath*, p) {
        const char* path = a_path_getFull(p);

        if(!a_path_test(p, A_PATH_FILE) || !a_str_endsWith(path, ".txt")) {
            continue;
        }

        a_template_new(path);
    }

    a_dir_free(dir);
}
