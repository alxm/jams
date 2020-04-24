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
    n_game.level = 0;
}

static void n_game_free(void)
{
    f_list_freeEx(n_game.orbs, (FFree*)o_orb_free);
}

static void setMap(int Width, int Height)
{
    n_game.size.x = Width;
    n_game.size.y = Height;
}

static void setPlayer(double X, double Y)
{
    n_game.player = o_orb_new(O_ORB_TYPE_PLAYER,
                              f_fix_fromDouble(X),
                              f_fix_fromDouble(Y),
                              F_DEG_090_INT);

    f_list_addLast(n_game.orbs, n_game.player);
}

static void setOrb(double X, double Y, OOrbTypeId Type)
{
    if(Type == O_ORB_TYPE_NPC_GOOD) {
        n_game.orbsGoodTotal++;
    }

    OOrb* o = o_orb_new(Type,
                        f_fix_fromDouble(X),
                        f_fix_fromDouble(Y),
                        f_random_intu(F_DEG_360_INT));

    f_list_addLast(n_game.orbs, o);
}

static void setOrbRandom(OOrbTypeId Type)
{
    if(Type == O_ORB_TYPE_NPC_GOOD) {
        n_game.orbsGoodTotal++;
    }

    OOrb* o = o_orb_new(Type,
                        f_random_int(f_fix_fromInt(n_game.size.x)),
                        f_random_int(f_fix_fromInt(n_game.size.y)),
                        f_random_intu(F_DEG_360_INT));

    f_list_addLast(n_game.orbs, o);
}

static const OOrbTypeId g_poison[] = {
    O_ORB_TYPE_NPC_POISON,
    O_ORB_TYPE_NPC_POISON2,
    O_ORB_TYPE_NPC_POISON3,
};

static inline OOrbTypeId randomPoison(unsigned Max)
{
    return g_poison[f_random_intu(Max)];
}

static void n_game_nextLevel(void)
{
    n_game.orbsGoodTotal = 0;

    f_list_clearEx(n_game.orbs, (FFree*)o_orb_free);

    switch(n_game.level++) {
        case 0: {
            setMap(2, 2);
            setPlayer(1, 1);

            setOrb(0.4, 0.4, O_ORB_TYPE_NPC_GOOD);
            setOrb(1.6, 0.4, O_ORB_TYPE_NPC_GOOD);
            setOrb(0.4, 1.6, O_ORB_TYPE_NPC_GOOD);
            setOrb(1.6, 1.6, O_ORB_TYPE_NPC_POISON);
        } break;

        case 1: {
            setMap(2, 2);
            setPlayer(1, 1);

            for(int i = 5; i--; ) {
                setOrbRandom(O_ORB_TYPE_NPC_GOOD);
                setOrbRandom(O_ORB_TYPE_NPC_POISON);
            }
        } break;

        case 2: {
            setMap(4, 3);
            setPlayer(2, 2.5);

            for(int i = 8; i--; ) {
                setOrbRandom(O_ORB_TYPE_NPC_GOOD);
            }

            for(int i = 4; i--; ) {
                setOrbRandom(randomPoison(2));
            }
        } break;

        case 3: {
            setMap(4, 3);
            setPlayer(2, 3);

            for(int i = 20; i--; ) {
                setOrbRandom(O_ORB_TYPE_NPC_GOOD);
            }

            for(int i = 8; i--; ) {
                setOrbRandom(randomPoison(3));
            }
        } break;
    }

    n_game.orbsGood = n_game.orbsGoodTotal;
}

static void t_game_dead(void)
{
    F_STATE_INIT
    {
        f_color_colorSetHex(0xa2a0ab);
        f_fade_startColorTo(200);

        f_state_popUntil(t_title);
        f_state_blockSet(f_fade_eventGet());
    }
}

static void t_game_next(void)
{
    F_STATE_INIT
    {
        n_game_nextLevel();

        f_fade_startScreens(400);

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

        f_color_colorSetHex(0xa2a0ab);
        f_fade_startColorFrom(200);
    }

    F_STATE_TICK
    {
        u_sound_tick();

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
