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

#include "state_game.h"

#include "component_position.h"
#include "obj_camera.h"
#include "obj_map.h"
#include "util_coords.h"
#include "util_ecs.h"
#include "util_input.h"

#define Z_MOVE_INC (16 * A_FIX_ONE / Z_COORDS_PIXELS_PER_UNIT)

typedef struct {
    AVectorFix coords;
    AEntity* player;
} NGame;

static NGame g_game;

static AEntity* newEntity(const char* Template, AVectorFix Coords)
{
    AEntity* e = a_entity_newEx(Template, NULL, &g_game);

    CPosition* pos = a_entity_componentAdd(e, U_COM_POSITION);
    c_position_init(pos, Coords);

    return e;
}

A_STATE(t_game)
{
    A_STATE_INIT
    {
        n_map_new();

        AVectorInt start = n_map_startGet();
        g_game.coords.x = a_fix_fromInt(start.x) + A_FIX_ONE / 2;
        g_game.coords.y = a_fix_fromInt(start.y) + A_FIX_ONE / 2;

        g_game.player = newEntity("actor.player", g_game.coords);
        n_camera_new(g_game.coords);
    }

    A_STATE_TICK
    {
        if(a_button_pressGet(u_input_get(U_BUTTON_UP))) {
            g_game.coords.y -= Z_MOVE_INC;
        }

        if(a_button_pressGet(u_input_get(U_BUTTON_DOWN))) {
            g_game.coords.y += Z_MOVE_INC;
        }

        if(a_button_pressGet(u_input_get(U_BUTTON_LEFT))) {
            g_game.coords.x -= Z_MOVE_INC;
        }

        if(a_button_pressGet(u_input_get(U_BUTTON_RIGHT))) {
            g_game.coords.x += Z_MOVE_INC;
        }

        #if Z_SHOW_MAP
            if(a_fps_ticksNth(A_CONFIG_FPS_RATE_TICK / 2)) {
                n_map_free();
                n_map_new();
            }
        #endif

        n_map_tick();
        n_camera_tick(g_game.coords);
        a_system_run(U_SYS_MOVE_TICK);
        a_system_run(U_SYS_SPRITE_TICK);
    }

    A_STATE_DRAW
    {
        n_map_draw();
        a_system_run(U_SYS_SPRITE_DRAW);
    }

    A_STATE_FREE
    {
        a_entity_removeSet(g_game.player);
        n_camera_free();
        n_map_free();
    }
}
