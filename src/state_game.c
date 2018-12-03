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

#include "entity_camera.h"
#include "entity_map.h"
#include "entity_player.h"

#include "util_ecs.h"

struct TGame {
    AEntity* camera;
    AEntity* map;
    AEntity* player;
    ATimer* turnTimer;
};

static void gameInit(TGame* Game)
{
    Game->camera = e_camera_new(Game);
    Game->map = e_map_new(Game, U_MAP_ID_FOREST);
    Game->player = e_player_new(Game, 8, 6);
    Game->turnTimer = a_timer_new(A_TIMER_MS, 250, false);
}

static void gameFree(TGame* Game)
{
    a_timer_free(Game->turnTimer);
}

static bool t_game_turnCanStart(const TGame* Game)
{
    return !a_timer_isRunning(Game->turnTimer);
}

static bool t_game_turnInProgress(const TGame* Game)
{
    return a_timer_isRunning(Game->turnTimer);
}

bool t_game_turnStart(const TGame* Game)
{
    if(a_timer_isRunning(Game->turnTimer)) {
        return false;
    }

    a_timer_start(Game->turnTimer);

    return true;
}

A_STATE(t_game)
{
    static TGame game;

    A_STATE_INIT
    {
        gameInit(&game);
    }

    A_STATE_TICK
    {
        if(t_game_turnCanStart(&game)) {
            a_system_run(U_SYS_INPUT);

            if(t_game_turnInProgress(&game)) {
                a_system_run(U_SYS_AI);
            }
        }

        a_system_run(U_SYS_CAMERA);
    }

    A_STATE_DRAW
    {
        a_pixel_colorSetHex(0x111111);
        a_draw_fill();

        a_system_run(U_SYS_MAPDRAW);
        a_system_run(U_SYS_SPRITEDRAW);
    }

    A_STATE_FREE
    {
        gameFree(&game);
    }
}

AEntity* t_game_getCamera(const TGame* Game)
{
    return Game->camera;
}

AEntity* t_game_getPlayer(const TGame* Game)
{
    return Game->player;
}
