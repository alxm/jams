/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "state_game.h"

#include "util_tiles.h"

#include "component_map.h"
#include "component_position.h"

#include "entity_alarm.h"
#include "entity_item.h"
#include "entity_log.h"
#include "entity_map.h"
#include "entity_pigeon.h"
#include "entity_player.h"

struct ZStateGame {
    AEntity* log;
    AEntity* map;
    AEntity* player;
    AEntity* alarm;
    AColMap* volumeColMap;
};

A_STATE(game)
{
    static ZStateGame game;

    A_STATE_INIT
    {
        a_system_tick("ttl input ai move animate logTick alarmTick");
        a_system_draw("mapDraw spriteDraw logDraw alarmDraw");

        game.log = z_entity_log_new();

        game.map = z_entity_map_new(&game, "gfx/level00.png");
        ZCompMap* map = a_entity_requireComponent(game.map, "map");

        int mapWidth, mapHeight;
        z_comp_map_getDim(map, &mapWidth, &mapHeight);

        game.volumeColMap = a_colmap_new(mapWidth * Z_UTIL_TILE_DIM,
                                           mapHeight * Z_UTIL_TILE_DIM,
                                           Z_UTIL_TILE_DIM);

        game.player = z_entity_player_new(&game, 2, 5);

        game.alarm = z_entity_alarm_new(&game);

        z_entity_item_new(&game, Z_ENTITY_ITEM_COFFER, 3, 5);
        z_entity_item_new(&game, Z_ENTITY_ITEM_COFFER, 5, 5);
        z_entity_item_new(&game, Z_ENTITY_ITEM_COFFER, 4, 6);

        z_entity_pigeon_new(&game, 4, 7);
        z_entity_pigeon_new(&game, 6, 7);
        z_entity_pigeon_new(&game, 6, 6);
    }

    A_STATE_BODY
    {
        A_STATE_LOOP;
    }

    A_STATE_FREE
    {
        a_colmap_free(game.volumeColMap);
    }
}

void z_state_game_getOrigin(const ZStateGame* Game, int* X, int* Y)
{
    ZCompPosition* pos = a_entity_requireComponent(Game->player, "position");

    AFix x, y;
    z_comp_position_getCoords(pos, &x, &y);

    *X = a_fix_fixtoi(x);
    *Y = a_fix_fixtoi(y);
}

AEntity* z_state_game_getLog(const ZStateGame* Game)
{
    return Game->log;
}

AEntity* z_state_game_getMap(const ZStateGame* Game)
{
    return Game->map;
}

AEntity* z_state_game_getPlayer(const ZStateGame* Game)
{
    return Game->player;
}

AEntity* z_state_game_getAlarm(const ZStateGame* Game)
{
    return Game->alarm;
}

AColMap* z_state_game_getVolumeColMap(const ZStateGame* Game)
{
    return Game->volumeColMap;
}

void z_state_game_gameOver(ZStateGame* Game)
{
    A_UNUSED(Game);
}
