/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Despot 3900 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Despot 3900 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Despot 3900.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_types.h"

#include "util_controls.h"
#include "util_despot.h"
#include "util_game.h"

ZGame* z_game;

static void game_checkGameOver(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    if(z_despot_getHealth(despot) <= 0 || z_despot_getWealth(despot) <= 0) {
        a_state_pop();
    }
}

A_STATE(game)
{
    A_STATE_INIT
    {
        z_game = z_game_init();
        z_game_log(z_game, NULL, "Hello, world");
    }

    A_STATE_BODY
    {
        z_controls_release();

        A_STATE_LOOP
        {
            if(z_game_logTick(z_game)) {
                z_game_setInstructions(z_game, "Wait...");
            } else {
                if(a_button_getPressedOnce(z_controls.action)) {
                    z_game_turn(z_game);
                } else {
                    z_game_setInstructions(z_game,
                                           "Press SPACE BAR for next turn");
                }
            }

            game_checkGameOver(z_game);

            A_STATE_LOOP_DRAW
            {
                z_game_draw(z_game);
            }
        }
    }

    A_STATE_FREE
    {
        z_game_free(z_game);
    }
}
