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

#include "entity_map.h"

#include "state_game.h"

#include "util_ecs.h"

struct TGame {
    AEntity* map;
};

static void zInitGame(TGame* Game)
{
    Game->map = e_map_new(Game, U_MAP_ID_CAVE);
}

static void zFreeGame(TGame* Game)
{
    a_entity_removeSet(Game->map);
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
        a_system_run(U_SYS_MAPDRAW);
    }

    A_STATE_FREE
    {
        zFreeGame(&game);
    }
}
