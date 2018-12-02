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

#include "state_game.h"

struct TGame {
    int x;
};

static void zInitGame(TGame* Game)
{
    Game->x = 1;
}

static void zFreeGame(TGame* Game)
{
    Game->x = 0;
}

A_STATE(t_game)
{
    static TGame game;

    A_STATE_INIT
    {
        zInitGame(&game);
    }

    A_STATE_TICK
    {
        //
    }

    A_STATE_DRAW
    {
        //
    }

    A_STATE_FREE
    {
        zFreeGame(&game);
    }
}
