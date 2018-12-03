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
#include "entity_enemy.h"
#include "entity_map.h"
#include "entity_player.h"

#include "macro_move.h"

#include "util_ecs.h"
#include "util_level.h"
#include "util_map.h"

struct TGame {
    unsigned level;
    ATimer* turnTimer;
    AEntity* player;
    AEntity* camera;
    UMapId activeMap;
    struct {
        ACollection* entities;
        AEntity* map;
    } maps[U_MAP_ID_NUM];
};

static void gameLevelLoadMap(TGame* Game, UMapId Map, const ABlock* Block)
{
    if(Block == NULL) {
        return;
    }

    Game->activeMap = Map;
    a_ecs_collectionSet(Game->maps[Map].entities);

    A_LIST_ITERATE(a_block_getAll(Block), const ABlock*, b) {
        TGameEntityContext context = {
            .template = a_block_readString(b, 0),
            .coords = a_block_readCoords(b, 1),
        };

        a_entity_muteInc(e_enemy_new(Game, &context));
    }

    Game->activeMap = U_MAP_ID_INVALID;
    a_ecs_collectionSet(NULL);
}

static void gameLevelLoad(TGame* Game)
{
    const ABlock* level = u_level_get(Game->level);

    const ABlock* cave = a_block_get(level, "cave");
    const ABlock* forest = a_block_get(level, "forest");

    gameLevelLoadMap(Game, U_MAP_ID_CAVE, cave);
    gameLevelLoadMap(Game, U_MAP_ID_FOREST, forest);
}

static void gameMapChange(TGame* Game, UMapId Map)
{
    if(Game->activeMap != U_MAP_ID_INVALID) {
        a_collection_muteInc(Game->maps[Game->activeMap].entities);
    }

    Game->activeMap = Map;
    a_collection_muteDec(Game->maps[Game->activeMap].entities);

    m_move_coordsSet(Game->player, (AVectorInt){8, 6});

    a_settings_colorSet(
        A_SETTING_COLOR_SCREEN_BORDER,
        u_map_getColorHex(e_map_mapGet(Game->maps[Game->activeMap].map)));

    a_fade_screens(500);
}

static void gameInit(TGame* Game)
{
    Game->level = 0;
    Game->turnTimer = a_timer_new(A_TIMER_MS, 250, false);
    Game->player = e_player_new(Game);
    Game->camera = e_camera_new(Game);

    for(UMapId m = U_MAP_ID_NUM; m--; ) {
        Game->maps[m].entities = a_collection_new();
        a_ecs_collectionSet(Game->maps[m].entities);

        Game->maps[m].map = e_map_new(Game, m);
        a_entity_muteInc(Game->maps[m].map);

        a_ecs_collectionSet(NULL);
    }

    gameLevelLoad(Game);

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
    return Game->maps[Game->activeMap].map;
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
