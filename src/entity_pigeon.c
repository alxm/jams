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

#include "util_controls.h"
#include "util_tiles.h"

#include "component_ai.h"
#include "component_map.h"
#include "component_motion.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_velocity.h"
#include "component_volume.h"

#include "entity_macros.h"

typedef enum {
    Z_PIGEON_AI_BLANK,
    Z_PIGEON_AI_WALKING,
} ZPigeonAiState;

typedef struct ZPigeonAiContext {
    ZPigeonAiState state;
    ZPigeonAiState oldState;
    int goalX, goalY;
    int moveCounter;
} ZPigeonAiContext;

static inline void setState(ZPigeonAiContext* Context, ZPigeonAiState State)
{
    Context->oldState = Context->state;
    Context->state = State;
}

static void pigeonAi(AEntity* Entity)
{
    ZCompAi* ai = a_entity_requireComponent(Entity, "ai");
    ZCompMotion* motion = a_entity_requireComponent(Entity, "motion");
    ZCompPosition* position = a_entity_requireComponent(Entity, "position");

    AFix x, y;
    z_comp_position_getCoords(position, &x, &y);

    int tileX = a_fix_fixtoi(x) / Z_UTIL_TILE_DIM;
    int tileY = a_fix_fixtoi(y) / Z_UTIL_TILE_DIM;

    ZStateGame* game = a_entity_getContext(Entity);
    ZCompMap* map = a_entity_requireComponent(z_state_game_getMap(game), "map");

    int mapWidth, mapHeight;
    z_comp_map_getDim(map, &mapWidth, &mapHeight);

    ZPigeonAiContext* ctx = z_comp_ai_getContext(ai);
    ZCompMotionState motionState = z_comp_motion_getState(motion);

    switch(ctx->state) {
        case Z_PIGEON_AI_BLANK: {
            int goalTileX = tileX;
            int goalTileY = tileY;;

            if(ctx->oldState != Z_PIGEON_AI_BLANK
                && motionState == Z_COMP_MOTION_STATE_OK
                && a_random_chance(3, 4)) {

                switch(z_comp_motion_getDirection(motion)) {
                    case Z_COMP_MOTION_DIR_UP: {
                        goalTileY--;
                    } break;

                    case Z_COMP_MOTION_DIR_DOWN: {
                        goalTileY++;
                    } break;

                    case Z_COMP_MOTION_DIR_LEFT: {
                        goalTileX--;
                    } break;

                    case Z_COMP_MOTION_DIR_RIGHT: {
                        goalTileX++;
                    } break;
                }
            } else {
                goalTileX += -1 + a_random_getInt(3);
                goalTileY += -1 + a_random_getInt(3);
            }

            if(goalTileX >= 0 && goalTileX < mapWidth
                && goalTileY >= 0 && goalTileY < mapHeight
                && (goalTileX != tileX || goalTileY != tileY)
                && z_comp_map_isWalkable(map, goalTileX, goalTileY)) {

                setState(ctx, Z_PIGEON_AI_WALKING);
                ctx->goalX = goalTileX * Z_UTIL_TILE_DIM + Z_UTIL_TILE_DIM / 2;
                ctx->goalY = goalTileY * Z_UTIL_TILE_DIM + Z_UTIL_TILE_DIM / 2;
            }

            ctx->moveCounter = 0;
        } break;

        case Z_PIGEON_AI_WALKING: {
            if(motionState == Z_COMP_MOTION_STATE_BLOCKED
                && ctx->moveCounter > Z_UTIL_TILE_DIM / 2) {

                setState(ctx, Z_PIGEON_AI_BLANK);
            } else {
                if(a_fix_fixtoi(x) < ctx->goalX) {
                    z_entity_macro_moveRight(Entity);
                } else if(a_fix_fixtoi(x) > ctx->goalX) {
                    z_entity_macro_moveLeft(Entity);
                } else if(a_fix_fixtoi(y) < ctx->goalY) {
                    z_entity_macro_moveDown(Entity);
                } else if(a_fix_fixtoi(y) > ctx->goalY) {
                    z_entity_macro_moveUp(Entity);
                } else {
                    setState(ctx, Z_PIGEON_AI_BLANK);
                }

                ctx->moveCounter++;
            }
        } break;
    }
}

AEntity* z_entity_pigeon_new(ZStateGame* Game, int TileX, int TileY)
{
    AEntity* e = a_entity_new("player", Game);

    AFix x = a_fix_itofix(TileX * Z_UTIL_TILE_DIM + Z_UTIL_TILE_DIM / 2);
    AFix y = a_fix_itofix(TileY * Z_UTIL_TILE_DIM + Z_UTIL_TILE_DIM / 2);

    ZCompAi* ai = a_entity_addComponent(e, "ai");
    z_comp_ai_init(ai, pigeonAi, sizeof(ZPigeonAiContext));

    a_entity_addComponent(e, "motion");

    ZCompPosition* position = a_entity_addComponent(e, "position");
    z_comp_position_init(position, x, y);

    ZCompSprite* sprite = a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite,
                       "pigeonUp",
                       "pigeonDown",
                       "pigeonLeft",
                       "pigeonRight");

    a_entity_addComponent(e, "velocity");

    ZCompVolume* volume = a_entity_addComponent(e, "volume");
    z_comp_volume_init(volume, z_state_game_getVolumeColMap(Game), x, y, 4);

    return e;
}
