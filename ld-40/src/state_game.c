/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "state_game.h"

#include "util_colors.h"
#include "util_coords.h"
#include "util_level.h"

#include "entity_building.h"
#include "entity_crystal.h"
#include "entity_cursor.h"
#include "entity_map.h"
#include "entity_worker.h"

struct ZStateGame {
    AColMap* volumeColMap;
    AEntity* map;
    AEntity* cursor;
    AEntity* base;
};

static void spawnCrystals(ZStateGame* Game, const ZUtilLevel* Level)
{
    int w, h;
    z_util_level_getDim(Level, &w, &h);

    for(int y = h; y--; ) {
        for(int x = w; x--; ) {
            APixel p = z_util_level_getValue(Level, x, y);

            for(int l = 0; l < Z_UTIL_COLOR_LEVELS; l++) {
                if(p != z_util_colors_get(Z_UTIL_COLOR_GREEN, l)) {
                    continue;
                }

                switch(l) {
                    case 0: {
                        z_entity_crystal_new(
                            Game,
                            z_util_coords_tileMid(x),
                            z_util_coords_tileMid(y));
                    } break;

                    case 1: {
                        z_entity_crystal_new(
                            Game,
                            z_util_coords_tileToInt(x) + 4,
                            z_util_coords_tileToInt(y) + 4);

                        z_entity_crystal_new(
                            Game,
                            z_util_coords_tileToInt(x) + 10,
                            z_util_coords_tileToInt(y) + 9);
                    } break;

                    case 2: {
                        z_entity_crystal_new(
                            Game,
                            z_util_coords_tileToInt(x) + 4,
                            z_util_coords_tileToInt(y) + 4);

                        z_entity_crystal_new(
                            Game,
                            z_util_coords_tileToInt(x) + 11,
                            z_util_coords_tileToInt(y) + 7);

                        z_entity_crystal_new(
                            Game,
                            z_util_coords_tileToInt(x) + 6,
                            z_util_coords_tileToInt(y) + 11);
                    } break;
                }

                break;
            }
        }
    }
}

static void spawnBuildings(ZStateGame* Game, const ZUtilLevel* Level)
{
    int w, h;
    z_util_level_getDim(Level, &w, &h);

    for(int y = h; y--; ) {
        for(int x = w; x--; ) {
            APixel p = z_util_level_getValue(Level, x, y);

            if(p == z_util_colors_get(Z_UTIL_COLOR_PINK, 2)) {
                int baseX = z_util_coords_tileMid(x);
                int baseY = z_util_coords_tileMid(y);

                Game->base = z_entity_building_new(Game,
                                                   Z_ENTITY_BUILDING_BASE,
                                                   baseX,
                                                   baseY);

                z_entity_worker_new(Game, baseX - 8, baseY + 12);
                z_entity_worker_new(Game, baseX + 8, baseY + 16);
                z_entity_worker_new(Game, baseX + 12, baseY + 4);
            } else if(p == z_util_colors_get(Z_UTIL_COLOR_PINK, 1)) {
                z_entity_building_new(Game,
                                      Z_ENTITY_BUILDING_DEPOT,
                                      z_util_coords_tileMid(x),
                                      z_util_coords_tileMid(y));
            } else if(p == z_util_colors_get(Z_UTIL_COLOR_PINK, 0)) {
                z_entity_building_new(Game,
                                      Z_ENTITY_BUILDING_TURRET,
                                      z_util_coords_tileMid(x),
                                      z_util_coords_tileMid(y));
            }
        }
    }
}

static void initGame(ZStateGame* Game)
{
    ZUtilLevel* level = z_util_level_load("gfx/map0.png");

    int w, h;
    z_util_level_getDim(level, &w, &h);

    w *= Z_UTIL_COORDS_TILE_DIM;
    h *= Z_UTIL_COORDS_TILE_DIM;

    Game->volumeColMap = a_colmap_new(w, h, Z_UTIL_COORDS_TILE_DIM);
    Game->map = z_entity_map_new(Game, level);
    Game->cursor = z_entity_cursor_new(Game);

    spawnCrystals(Game, level);
    spawnBuildings(Game, level);

    z_util_level_free(level);
}

static void freeGame(ZStateGame* Game)
{
    a_colmap_free(Game->volumeColMap);
}

AColMap* z_state_game_getVolumeColMap(const ZStateGame* Game)
{
    return Game->volumeColMap;
}

AEntity* z_state_game_getMap(const ZStateGame* Game)
{
    return Game->map;
}

AEntity* z_state_game_getBase(const ZStateGame* Game)
{
    return Game->base;
}

A_STATE(game)
{
    static ZStateGame game;

    A_STATE_INIT
    {
        initGame(&game);
    }

    A_STATE_FREE
    {
        freeGame(&game);
    }
}
