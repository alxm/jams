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

NGame n_game;

static void n_game_new(void)
{
    n_game.orbs = f_list_new();
}

static void n_game_free(void)
{
    f_list_freeEx(n_game.orbs, (FFree*)o_orb_free);
}

static void n_game_nextLevel(void)
{
    int level = n_game.level++;
    F_UNUSED(level);

    n_game.orbsGood = 0;

    f_list_clearEx(n_game.orbs, (FFree*)o_orb_free);

    for(int i = 10; i--; ) {
        OOrbTypeId type = f_random_chance(4, 5)
                                ? O_ORB_TYPE_NPC_GOOD
                                : O_ORB_TYPE_NPC_POISON;

        if(type == O_ORB_TYPE_NPC_GOOD) {
            n_game.orbsGood++;
        }

        OOrb* o = o_orb_new(type,
                            f_random_range(0, f_fix_fromInt(N_MAP_W)),
                            f_random_range(0, f_fix_fromInt(N_MAP_H)),
                            f_random_intu(F_DEG_360_INT));

        f_list_addLast(n_game.orbs, o);
    }

    n_game.player = o_orb_new(O_ORB_TYPE_PLAYER,
                              f_fix_fromInt(N_MAP_W / 2),
                              f_fix_fromInt(N_MAP_H / 2),
                              F_DEG_090_INT);

    f_list_addLast(n_game.orbs, n_game.player);
}

static void t_game_dead(void)
{
    F_STATE_INIT
    {
        f_state_popUntil(t_title);
    }
}

static void t_game_next(void)
{
    F_STATE_INIT
    {
        n_game_nextLevel();

        f_state_pop();
    }
}

void t_game_play(void)
{
    F_STATE_INIT
    {
        n_game_new();
        n_cam_new();

        n_game_nextLevel();
    }

    F_STATE_TICK
    {
        F_LIST_ITERATE(n_game.orbs, OOrb*, o) {
            o_orb_tick(o);

            if(o->state.id == O_ORB_STATE_DEAD) {
                F_LIST_REMOVE_CURRENT();
                o_orb_free(o);
            }
        }

        n_cam_tick(n_game.player->coords);

        n_game.player->life -= 2;

        if(n_game.player->life <= 0) {
            f_state_push(t_game_dead);
        } else if(n_game.orbsGood == 0) {
            f_state_push(t_game_next);
        }
    }

    F_STATE_DRAW
    {
        n_map_draw();

        F_LIST_ITERATE(n_game.orbs, OOrb*, o) {
            o_orb_draw0(o);
        }

        F_LIST_ITERATE(n_game.orbs, OOrb*, o) {
            o_orb_draw(o);
        }

        n_hud_draw();
    }

    F_STATE_FREE
    {
        n_game_free();
        n_cam_free();
    }
}
