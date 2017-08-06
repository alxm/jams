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
#include "component_position.h"
#include "component_sprite.h"
#include "component_velocity.h"
#include "component_volume.h"

#include "entity_macros.h"

typedef enum {
    Z_PERSON_AI_BLANK,
    Z_PERSON_AI_WALKING,
} ZPersonAiState;

typedef struct ZPersonAiContext {
    ZPersonAiState state;
    int goalTileX, goalTileY;
} ZPersonAiContext;

static void person_walkAroundAi(AEntity* Entity)
{
    ZCompAi* ai = a_entity_requireComponent(Entity, "ai");
    ZCompPosition* position = a_entity_requireComponent(Entity, "position");

    AFix x, y;
    z_comp_position_getCoords(position, &x, &y);

    int tileX = a_fix_fixtoi(x) / Z_UTIL_TILE_DIM;
    int tileY = a_fix_fixtoi(y) / Z_UTIL_TILE_DIM;

    ZStateGame* game = a_entity_getContext(Entity);
    ZCompMap* map = a_entity_requireComponent(z_state_game_getMap(game), "map");

    int mapWidth, mapHeight;
    z_comp_map_getDim(map, &mapWidth, &mapHeight);

    ZPersonAiContext* ctx = z_comp_ai_getContext(ai);

    switch(ctx->state) {
        case Z_PERSON_AI_BLANK: {
            /* TODO:
             * - 75% chance to pick previous direction instead of random
             * - if collides with something, reverse course
             * - move at least half a tile before changing direction
             * - if did not move at all for the past X frames, go to blank state
             */

            int newTileX = tileX - 1 + a_random_getInt(3);
            int newTileY = tileY - 1 + a_random_getInt(3);

            if(newTileX >= 0 && newTileX < mapWidth
                && newTileY >= 0 && newTileY < mapHeight
                && (newTileX != tileX || newTileY != tileY)
                && z_comp_map_isWalkable(map, newTileX, newTileY)) {

                ctx->state = Z_PERSON_AI_WALKING;
                ctx->goalTileX = newTileX;
                ctx->goalTileY = newTileY;
            }
        } break;

        case Z_PERSON_AI_WALKING: {
            if(tileX == ctx->goalTileX && tileY == ctx->goalTileY) {
                ctx->state = Z_PERSON_AI_BLANK;
            } else {
                if(tileX < ctx->goalTileX) {
                    z_entity_macro_moveRight(Entity);
                } else if(tileX > ctx->goalTileX) {
                    z_entity_macro_moveLeft(Entity);
                } else if(tileY < ctx->goalTileY) {
                    z_entity_macro_moveDown(Entity);
                } else if(tileY > ctx->goalTileY) {
                    z_entity_macro_moveUp(Entity);
                }
            }
        } break;
    }
}

AEntity* z_entity_person_new(ZStateGame* Game, int TileX, int TileY)
{
    AEntity* e = a_entity_new("player", Game);

    AFix x = a_fix_itofix(TileX * Z_UTIL_TILE_DIM + Z_UTIL_TILE_DIM / 2);
    AFix y = a_fix_itofix(TileY * Z_UTIL_TILE_DIM + Z_UTIL_TILE_DIM / 2);

    ZCompAi* ai = a_entity_addComponent(e, "ai");
    z_comp_ai_init(ai, person_walkAroundAi, sizeof(ZPersonAiContext));
    ZPersonAiContext* ctx = z_comp_ai_getContext(ai);
    ctx->state = Z_PERSON_AI_BLANK;

    ZCompPosition* position = a_entity_addComponent(e, "position");
    z_comp_position_init(position, x, y);

    ZCompSprite* sprite = a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite,
                       "personUp",
                       "personDown",
                       "personLeft",
                       "personRight");

    a_entity_addComponent(e, "velocity");

    ZCompVolume* volume = a_entity_addComponent(e, "volume");
    z_comp_volume_init(volume, z_state_game_getVolumeColMap(Game), x, y, 4);

    return e;
}
