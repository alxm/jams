/*
    Copyright 2020 Alex Margarit <alex@alxm.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "t_game.h"
#include "main.h"

typedef struct {
    FList* orbs;
    OOrb* player;
} NGame;

static NGame g_game;

void t_game(void)
{
    F_STATE_INIT
    {
        g_game.orbs = f_list_new();

        for(int i = 100; i--; ) {
            OOrb* o = o_orb_new(O_ORB_TYPE_NPC1 + f_random_int(2),
                                f_random_range(0, f_fix_fromInt(N_MAP_W)),
                                f_random_range(0, f_fix_fromInt(N_MAP_H)),
                                f_random_intu(F_FIX_ANGLES_NUM));

            f_list_addLast(g_game.orbs, o);
        }

        g_game.player = o_orb_new(O_ORB_TYPE_PLAYER,
                                  f_fix_fromInt(N_MAP_W / 2),
                                  f_fix_fromInt(N_MAP_H / 2),
                                  F_DEG_090_INT);

        n_map_new();
        n_cam_new();
    }

    F_STATE_TICK
    {
        n_map_tick();

        o_orb_tick(g_game.player);

        F_LIST_ITERATE(g_game.orbs, OOrb*, o) {
            o_orb_tick(o);
        }

        n_cam_tick(g_game.player->coords);
    }

    F_STATE_DRAW
    {
        n_map_draw();

        F_LIST_ITERATE(g_game.orbs, OOrb*, o) {
            o_orb_draw(o);
        }

        o_orb_draw(g_game.player);
    }

    F_STATE_FREE
    {
        F_LIST_ITERATE(g_game.orbs, OOrb*, o) {
            o_orb_free(o);
        }

        n_map_free();

        f_list_free(g_game.orbs);
    }
}

OOrb* t_game_getPlayer(void)
{
    return g_game.player;
}
