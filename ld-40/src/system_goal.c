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

#include "util_coords.h"
#include "util_level.h"
#include "util_terrain.h"

#include "component_goal.h"
#include "component_mapbuildings.h"
#include "component_mapterrain.h"
#include "component_position.h"
#include "component_volume.h"

#include "entity_macros.h"

typedef struct {
    void* parentCoords;
    int fromStartToHere;
    int estFromHereToEnd;
    bool inClosedList;
    bool inOpenList;
} ZTile;

static inline void* pointPack(int X, int Y)
{
    return (void*)(ptrdiff_t)(((Y & 0xffff) << 16) | (X & 0xffff));
}

static inline void pointUnpack(void* Point, int* X, int *Y)
{
    *X = (ptrdiff_t)Point & 0xffff;
    *Y = ((ptrdiff_t)Point >> 16) & 0xffff;
}

static void nop(AEntity* Entity, ZCompGoal* Goal)
{
    A_UNUSED(Entity);
    A_UNUSED(Goal);
}

static bool aStar(ZCompMapTerrain* Terrain, ZCompMapBuildings* Buildings, int StartX, int StartY, int EndX, int EndY, int* NextX, int* NextY)
{
    bool foundPath = false;

    *NextX = StartX;
    *NextY = StartY;

    int mapWidth, mapHeight;
    z_comp_mapterrain_getDim(Terrain, &mapWidth, &mapHeight);

    const ZUtilTerrainType** terrainMap = z_comp_mapterrain_getMap(Terrain);

    ZTile tiles[mapHeight][mapWidth];

    for(int y = mapHeight; y--; ) {
        for(int x = mapWidth; x--; ) {
            tiles[y][x].parentCoords = 0;
            tiles[y][x].fromStartToHere = INT_MAX;
            tiles[y][x].estFromHereToEnd = INT_MAX;
            tiles[y][x].inClosedList = false;
            tiles[y][x].inOpenList = false;
        }
    }

    AList* openList = a_list_new();
    ZTile* startTile = &tiles[StartY][StartX];

    a_list_addLast(openList, pointPack(StartX, StartY));
    startTile->fromStartToHere = 0;
    startTile->estFromHereToEnd = a_math_abs(EndX - StartX)
                                    + a_math_abs(EndY - StartY);
    startTile->inOpenList = true;

    while(!a_list_isEmpty(openList)) {
        int lowestScore = INT_MAX;
        ZTile* currentTile = NULL;
        void* currentTileCoords = NULL;

        A_LIST_ITERATE(openList, void*, tileCoords) {
            int tileX, tileY;
            pointUnpack(tileCoords, &tileX, &tileY);

            ZTile* tile = &tiles[tileY][tileX];
            int score = tile->fromStartToHere + tile->estFromHereToEnd;

            if(score < lowestScore) {
                if(currentTile != NULL) {
                    a_list_addFirst(openList, currentTileCoords);
                }

                lowestScore = score;
                currentTile = tile;
                currentTileCoords = tileCoords;

                A_LIST_REMOVE_CURRENT();

                if(tileX == EndX && tileY == EndY) {
                    int lastTileX = tileX;
                    int lastTileY = tileY;

                    while(true) {
                        if(tileX == StartX && tileY == StartY) {
                            *NextX = lastTileX;
                            *NextY = lastTileY;

                            foundPath = true;
                            goto done;
                        }

                        lastTileX = tileX;
                        lastTileY = tileY;

                        pointUnpack(tile->parentCoords, &tileX, &tileY);
                        tile = &tiles[tileY][tileX];
                    }
                }
            }
        }

        currentTile->inClosedList = true;

        int currentTileX, currentTileY;
        pointUnpack(currentTileCoords, &currentTileX, &currentTileY);

        #define checkTile(X, Y)                                              \
        {                                                                    \
            int tileX = X;                                                   \
            int tileY = Y;                                                   \
            ZTile* tile = &tiles[tileY][tileX];                              \
            ZUtilTerrainType terrain = terrainMap[tileY][tileX];             \
                                                                             \
            if(z_util_terrain_isWalkable(terrain)                            \
                && z_comp_mapbuildings_get(Buildings, tileX, tileY) == NULL  \
                && !tile->inClosedList) {                                    \
                                                                             \
                int fromStartToHere = currentTile->fromStartToHere + 1;      \
                int estFromHereToEnd = a_math_abs(EndX - tileX)              \
                                        + a_math_abs(EndY - tileY);          \
                int score = fromStartToHere + estFromHereToEnd;              \
                int tScore = tile->fromStartToHere + tile->estFromHereToEnd; \
                                                                             \
                                                                             \
                if(!tile->inOpenList || score < tScore) {                    \
                    tile->parentCoords = currentTileCoords;                  \
                    tile->fromStartToHere = fromStartToHere;                 \
                    tile->estFromHereToEnd = estFromHereToEnd;               \
                                                                             \
                    if(!tile->inOpenList) {                                  \
                        tile->inOpenList = true;                             \
                        a_list_addLast(openList, pointPack(tileX, tileY));   \
                    }                                                        \
                }                                                            \
            }                                                                \
        }

        if(currentTileY > 0) {
            checkTile(currentTileX, currentTileY - 1);
        }

        if(currentTileY < mapHeight - 1) {
            checkTile(currentTileX, currentTileY + 1);
        }

        if(currentTileX > 0) {
            checkTile(currentTileX - 1, currentTileY);
        }

        if(currentTileX < mapWidth - 1) {
            checkTile(currentTileX + 1, currentTileY);
        }
    }

done:
    a_list_free(openList);

    return foundPath;
}

static void pathfind(AEntity* Entity, ZCompGoal* Goal)
{
    ZCompPosition* position = a_entity_reqComponent(Entity, "position");

    int currentX, currentY;
    z_comp_position_getCoordsInt(position, &currentX, &currentY);

    int destX, destY;
    z_comp_goal_getDestCoords(Goal, &destX, &destY);

    int currentTileX = z_util_coords_intToTileInt(currentX);
    int currentTileY = z_util_coords_intToTileInt(currentY);
    int destTileX = z_util_coords_intToTileInt(destX);
    int destTileY = z_util_coords_intToTileInt(destY);

    if(currentX == destX && currentY == destY) {
        AEntity* objective = z_comp_goal_getObjective(Goal);

        if(objective && a_entity_hasComponent(objective, "tagCrystal")) {
            z_comp_goal_setState(Goal, Z_COMP_GOAL_STATE_MINE);
        } else if(!z_comp_goal_pop(Goal)) {
            z_comp_goal_setState(Goal, Z_COMP_GOAL_STATE_NONE);
        } else {
            z_comp_goal_setState(Goal, Z_COMP_GOAL_STATE_PATHFINDING);
        }

        return;
    }

    ZStateGame* game = a_entity_getContext(Entity);
    AEntity* map = z_state_game_getMap(game);
    int nextTileX, nextTileY;

    bool foundPath = aStar(a_entity_reqComponent(map, "mapTerrain"),
                           a_entity_reqComponent(map, "mapBuildings"),
                           currentTileX,
                           currentTileY,
                           destTileX,
                           destTileY,
                           &nextTileX,
                           &nextTileY);

    int targetX, targetY;

    if(foundPath) {
        targetX = z_util_coords_tileMid(nextTileX);
        targetY = z_util_coords_tileMid(nextTileY);
    } else if(z_comp_goal_hasNextCoords(Goal)) {
        z_comp_goal_getNextCoords(Goal, &targetX, &targetY);
    } else {
        z_comp_goal_clearNextCoords(Goal);
        return;
    }

    if(nextTileX == destTileX && nextTileY == destTileY) {
        targetX = destX;
        targetY = destY;
    }

    z_comp_goal_setNextCoords(Goal, targetX, targetY);

    if(currentX < targetX) {
        z_entity_macro_moveRight(Entity);
    } else if(currentX > targetX) {
        z_entity_macro_moveLeft(Entity);
    }

    if(currentY < targetY) {
        z_entity_macro_moveDown(Entity);
    } else if(currentY > targetY) {
        z_entity_macro_moveUp(Entity);
    }
}

static void mine(AEntity* Entity, ZCompGoal* Goal)
{
    A_UNUSED(Entity);

    // Remember that we're mining
    z_comp_goal_push(Goal);

    z_comp_goal_setState(Goal, Z_COMP_GOAL_STATE_BRINGBACK);
}

static void bringback(AEntity* Entity, ZCompGoal* Goal)
{
    ZStateGame* game = a_entity_getContext(Entity);
    AEntity* base = z_state_game_getBase(game);
    ZCompPosition* basePos = a_entity_reqComponent(base, "position");

    int baseX, baseY;
    z_comp_position_getCoordsInt(basePos, &baseX, &baseY);

    z_comp_goal_setState(Goal, Z_COMP_GOAL_STATE_PATHFINDING);
    z_comp_goal_setDestCoords(Goal, baseX, baseY + Z_UTIL_COORDS_TILE_DIM);
}

void (*g_goalCallbacks[Z_COMP_GOAL_STATE_NUM])(AEntity*, ZCompGoal*) = {
    nop,
    pathfind,
    mine,
    bringback,
};

void z_system_goalTick(AEntity* Entity)
{
    ZCompGoal* goal = a_entity_reqComponent(Entity, "goal");

    g_goalCallbacks[z_comp_goal_getState(goal)](Entity, goal);
}
