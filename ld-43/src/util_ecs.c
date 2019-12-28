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

#include "component_ai.h"
#include "component_damage.h"
#include "component_health.h"
#include "component_input.h"
#include "component_map.h"
#include "component_position.h"
#include "component_sprite.h"

#include "system_ai.h"
#include "system_camera.h"
#include "system_health.h"
#include "system_input.h"
#include "system_mapdraw.h"
#include "system_spritedraw.h"

static void u_components_load(void)
{
    c_ai_register(U_COM_AI);
    a_component_new(U_COM_CAMERA, "camera", 0, NULL, NULL);
    c_damage_register(U_COM_DAMAGE);
    c_health_register(U_COM_HEALTH);
    c_input_register(U_COM_INPUT);
    c_map_register(U_COM_MAP);
    c_position_register(U_COM_POSITION);
    c_sprite_register(U_COM_SPRITE);
}

static void u_systems_load(void)
{
    s_ai_register(U_SYS_AI);
    s_camera_register(U_SYS_CAMERA);
    s_health_register(U_SYS_HEALTH);
    s_input_register(U_SYS_INPUT);
    s_mapdraw_register(U_SYS_MAPDRAW);
    s_spritedraw_register(U_SYS_SPRITEDRAW);
}

static void u_templates_load(void)
{
    ADir* root = a_dir_new("assets/entities");

    A_LIST_ITERATE(a_dir_entriesListGet(root), APath*, p) {
        if(!a_path_test(p, A_PATH_DIR)) {
            continue;
        }

        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%s/info.txt", a_path_getFull(p));

        a_template_new(buffer, NULL);
    }

    a_dir_free(root);
}

void u_ecs_load(void)
{
    a_ecs_init(U_COM_NUM, U_SYS_NUM, U_MSG_NUM);

    u_components_load();
    u_systems_load();
    u_templates_load();
}
