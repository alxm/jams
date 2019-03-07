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

#include "obj_camera.h"
#include "obj_map.h"
#include "util_coords.h"
#include "util_input.h"

#define Z_MOVE_INC (4 * A_FIX_ONE / Z_COORDS_PIXELS_PER_UNIT)

typedef struct {
    AVectorFix coords;
} NGame;

static NGame g_game;

A_STATE(t_game)
{
    A_STATE_INIT
    {
        g_game.coords.x = a_fix_fromInt(N_MAP_W / 2) + A_FIX_ONE / 2;
        g_game.coords.y = a_fix_fromInt(N_MAP_H / 2) + A_FIX_ONE / 2;

        n_camera_new(g_game.coords);
        n_map_new();
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

        if(a_fps_ticksNth(A_CONFIG_FPS_RATE_TICK / 2)) {
            n_map_free();
            n_map_new();
        }

        n_map_tick();
        n_camera_tick(g_game.coords);
    }

    A_STATE_DRAW
    {
        n_map_draw();
    }

    A_STATE_FREE
    {
        n_camera_free();
        n_map_free();
    }
}
