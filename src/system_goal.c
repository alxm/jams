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
#include "component_mapterrain.h"
#include "component_position.h"
#include "component_volume.h"

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

static bool aStar(ZCompMapTerrain* Terrain, int StartX, int StartY, int EndX, int EndY, int* NextX, int* NextY)
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
            if(z_util_terrain_isWalkable(terrain) && !tile->inClosedList) {  \
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

void z_system_goalTick(AEntity* Entity)
{
    ZCompGoal* goal = a_entity_reqComponent(Entity, "goal");

    if(z_comp_goal_getState(goal) == Z_COMP_GOAL_STATE_NONE) {
        return;
    }

    ZCompPosition* position = a_entity_reqComponent(Entity, "position");

    int currentX, currentY;
    z_comp_position_getCoordsInt(position, &currentX, &currentY);

    int destX, destY;
    z_comp_goal_getDestCoords(goal, &destX, &destY);

    int currentTileX = z_util_coords_intToTileInt(currentX);
    int currentTileY = z_util_coords_intToTileInt(currentY);
    int destTileX = z_util_coords_intToTileInt(destX);
    int destTileY = z_util_coords_intToTileInt(destY);

    if(currentX == destX && currentY == destY) {
        z_comp_goal_setState(goal, Z_COMP_GOAL_STATE_NONE);
    } else {
        ZStateGame* game = a_entity_getContext(Entity);
        AEntity* map = z_state_game_getMap(game);
        ZCompMapTerrain* terrain = a_entity_reqComponent(map, "mapTerrain");
        int nextTileX, nextTileY;

        bool foundPath = aStar(terrain,
                               currentTileX,
                               currentTileY,
                               destTileX,
                               destTileY,
                               &nextTileX,
                               &nextTileY);

        if(!foundPath) {
            return;
        }

        int targetX = z_util_coords_tileMid(nextTileX);
        int targetY = z_util_coords_tileMid(nextTileY);
        z_comp_goal_setNextCoords(goal, targetX, targetY);

        AFix xf, yf;
        z_comp_position_getCoordsFix(position, &xf, &yf);

        if(currentX < targetX) {
            xf += A_FIX_ONE / 2;
        } else if(currentX > targetX) {
            xf -= A_FIX_ONE / 2;
        }

        if(currentY < targetY) {
            yf += A_FIX_ONE / 2;
        } else if(currentY > targetY) {
            yf -= A_FIX_ONE / 2;
        }

        ZCompVolume* volume = a_entity_reqComponent(Entity, "volume");

        z_comp_position_setCoords(position, xf, yf);
        z_comp_volume_setCoords(volume, xf, yf);
    }
}
