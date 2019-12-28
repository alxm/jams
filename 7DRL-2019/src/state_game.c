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
#include "component_sprite.h"
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

A_STATE(t_game)
{
    A_STATE_INIT
    {
        n_map_new();

        AVectorInt start = n_map_startGet();

        n_camera_new(start);

        g_game.player = a_entity_newEx("actor.player", NULL, &g_game);

        CPosition* pos = a_entity_componentAdd(g_game.player, U_COM_POSITION);
        c_position_init(pos, start);

        for(int i = U_BUTTON_UP; i <= U_BUTTON_RIGHT; i++) {
            a_button_pressSetRepeat(u_input_get(i), 200);
        }
    }

    A_STATE_TICK
    {
        CPosition* pos = a_entity_componentReq(g_game.player, U_COM_POSITION);
        CSprite* spr = a_entity_componentReq(g_game.player, U_COM_SPRITE);
        AVectorInt coords = c_position_coordsGet(pos);

        if(a_button_pressGet(u_input_get(U_BUTTON_UP))) {
            if(n_map_canWalk(coords.x, coords.y - 1)) {
                coords.y--;
            }

            c_sprite_directionSet(spr, C_SPRITE_DIR_UP);
        }

        if(a_button_pressGet(u_input_get(U_BUTTON_DOWN))) {
            if(n_map_canWalk(coords.x, coords.y + 1)) {
                coords.y++;
            }

            c_sprite_directionSet(spr, C_SPRITE_DIR_DOWN);
        }

        if(a_button_pressGet(u_input_get(U_BUTTON_LEFT))) {
            if(n_map_canWalk(coords.x - 1, coords.y)) {
                coords.x--;
            }

            c_sprite_directionSet(spr, C_SPRITE_DIR_LEFT);
        }

        if(a_button_pressGet(u_input_get(U_BUTTON_RIGHT))) {
            if(n_map_canWalk(coords.x + 1, coords.y)) {
                coords.x++;
            }

            c_sprite_directionSet(spr, C_SPRITE_DIR_RIGHT);
        }

        c_position_coordsSet(pos, coords);

        n_map_tick();
        n_camera_tick(coords);
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
