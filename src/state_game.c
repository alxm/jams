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

#include "macro_move.h"

#include "util_ecs.h"
#include "util_map.h"

struct TGame {
    ATimer* turnTimer;
    AEntity* player;
    AEntity* camera;
    AEntity* maps[U_MAP_ID_NUM];
    UMapId activeMap;
};

static void gameMapChange(TGame* Game, UMapId Map)
{
    if(Game->activeMap != U_MAP_ID_INVALID) {
        a_entity_muteInc(Game->maps[Game->activeMap]);
    }

    Game->activeMap = Map;
    a_entity_muteDec(Game->maps[Game->activeMap]);

    m_move_coordsSet(Game->player, 8, 6);
}

static void gameInit(TGame* Game)
{
    Game->turnTimer = a_timer_new(A_TIMER_MS, 250, false);
    Game->camera = e_camera_new(Game);
    Game->player = e_player_new(Game);

    for(UMapId m = U_MAP_ID_NUM; m--; ) {
        Game->maps[m] = e_map_new(Game, m);
        a_entity_muteInc(Game->maps[m]);
    }

    Game->activeMap = U_MAP_ID_INVALID;
    gameMapChange(Game, U_MAP_ID_CAVE);
}

static void gameFree(TGame* Game)
{
    a_timer_free(Game->turnTimer);
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
        if(!a_timer_isRunning(game.turnTimer)) {
            a_system_run(U_SYS_INPUT);

            if(a_timer_isRunning(game.turnTimer)) {
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

AEntity* t_game_getPlayer(const TGame* Game)
{
    return Game->player;
}

AEntity* t_game_getCamera(const TGame* Game)
{
    return Game->camera;
}

AEntity* t_game_getMap(const TGame* Game)
{
    return Game->maps[Game->activeMap];
}

bool t_game_turnStart(const TGame* Game)
{
    if(a_timer_isRunning(Game->turnTimer)) {
        return false;
    }

    a_timer_start(Game->turnTimer);

    return true;
}

void t_game_runCode(TGame* Game, int Code)
{
    switch(Code) {
        case 1: {
            gameMapChange(Game, U_MAP_ID_FOREST);
        } break;

        case 2: {
            gameMapChange(Game, U_MAP_ID_CAVE);
        } break;
    }
}
