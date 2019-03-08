/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of City, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "obj_map.h"

#include "obj_camera.h"
#include "util_coords.h"
#include "util_tile.h"

typedef enum {
    Z_TILE_FLAG_VISITED = A_FLAG_BIT(1),
} ZTileFlags;

typedef enum {
    Z_ROAD_UP,
    Z_ROAD_DOWN,
    Z_ROAD_LEFT,
    Z_ROAD_RIGHT,
    Z_ROAD_NUM
} ZAreaRoadId;

typedef struct {
    int size;
} ZAreaRoad;

typedef struct {
    int x, y, w, h;
    ZAreaRoad road[Z_ROAD_NUM];
} ZArea;

typedef struct {
    UTile id;
    unsigned flags;
} ZTile;

typedef struct {
    AList* areas;
    ZTile tiles[N_MAP_H][N_MAP_W];
} NMap;

static NMap g_map;

static ZArea* z_area_new(int X, int Y, int W, int H)
{
    ZArea* a = a_mem_zalloc(sizeof(ZArea));

    a->x = X;
    a->y = Y;
    a->w = W;
    a->h = H;

    return a;
}

static void z_area_free(ZArea* Area)
{
    free(Area);
}

static AList* z_area_subdivide(ZArea* StartArea, int BlockSizeMin, int RoadSize, int Iterations)
{
    AList* areas = a_list_new();

    a_list_addLast(areas, StartArea);

    while(Iterations--) {
        ZArea* areaOld = NULL;

        for(int tries = 8; tries--; ) {
            ZArea* a = a_list_getRandom(areas);

            if(a->w >= 2 * BlockSizeMin + RoadSize
                || a->h >= 2 * BlockSizeMin + RoadSize) {

                areaOld = a;
                break;
            }
        }

        if(areaOld == NULL) {
            A_LIST_ITERATE(areas, ZArea*, a) {
                if(a->w >= 2 * BlockSizeMin + RoadSize
                    || a->h >= 2 * BlockSizeMin + RoadSize) {

                    areaOld = a;
                    break;
                }
            }
        }

        if(areaOld == NULL) {
            break;
        }

        ZArea* areaNew;
        bool splitAlongHeight = areaOld->w == areaOld->h
                                    ? a_random_int(2) : areaOld->w > areaOld->h;

        if(splitAlongHeight) {
            int smallest = a_math_max(
                            BlockSizeMin + (RoadSize >> 1), areaOld->w / 4);
            int median = a_random_range(smallest, areaOld->w - smallest + 1);

            areaNew = z_area_new(areaOld->x + median,
                                 areaOld->y,
                                 areaOld->w - median,
                                 areaOld->h);

            areaOld->w = median;
            memcpy(areaNew->road, areaOld->road, sizeof(areaOld->road));

            if(areaOld->w > areaNew->w) {
                areaOld->road[Z_ROAD_RIGHT].size = RoadSize - (RoadSize >> 1);
                areaNew->road[Z_ROAD_LEFT].size = RoadSize >> 1;
            } else {
                areaOld->road[Z_ROAD_RIGHT].size = RoadSize >> 1;
                areaNew->road[Z_ROAD_LEFT].size = RoadSize - (RoadSize >> 1);
            }
        } else {
            int smallest = a_math_max(
                            BlockSizeMin + (RoadSize >> 1), areaOld->h / 4);
            int median = a_random_range(smallest, areaOld->h - smallest + 1);

            areaNew = z_area_new(areaOld->x,
                                 areaOld->y + median,
                                 areaOld->w,
                                 areaOld->h - median);

            areaOld->h = median;
            memcpy(areaNew->road, areaOld->road, sizeof(areaOld->road));

            if(areaOld->h > areaNew->h) {
                areaOld->road[Z_ROAD_DOWN].size = RoadSize - (RoadSize >> 1);
                areaNew->road[Z_ROAD_UP].size = RoadSize >> 1;
            } else {
                areaOld->road[Z_ROAD_DOWN].size = RoadSize >> 1;
                areaNew->road[Z_ROAD_UP].size = RoadSize - (RoadSize >> 1);
            }
        }

        a_list_addLast(areas, areaNew);
    }

    return areas;
}

static inline int z_area_surfaceGet(const ZArea* Area)
{
    return Area->w * Area->h;
}

static inline int z_area_distanceGetSq(const ZArea* Area)
{
    int x = Area->x + Area->w / 2 - N_MAP_W / 2;
    int y = Area->y + Area->h / 2 - N_MAP_H / 2;

    return x * x + y * y;
}

static int z_area_cmpSizeInv(void* ItemA, void* ItemB)
{
    return z_area_surfaceGet(ItemB) - z_area_surfaceGet(ItemA);
}

static int z_area_cmpDistance(void* ItemA, void* ItemB)
{
    return z_area_distanceGetSq(ItemA) - z_area_distanceGetSq(ItemB);
}

static int z_area_cmpDistanceInv(void* ItemA, void* ItemB)
{
    return z_area_distanceGetSq(ItemB) - z_area_distanceGetSq(ItemA);
}

static void mapGenAreasDivideN(NMap* Map, AListCompare* SortCallback, unsigned Num, int BlockSizeMin, int RoadSize, int Iterations)
{
    Num = a_math_minu(Num, a_list_sizeGet(Map->areas));

    if(SortCallback && Num > 1) {
        a_list_sort(Map->areas, SortCallback);
    }

    while(Num--) {
        AList* subAreas = z_area_subdivide(a_list_pop(Map->areas),
                                           BlockSizeMin,
                                           RoadSize,
                                           Iterations);

        a_list_appendMove(Map->areas, subAreas);
        a_list_free(subAreas);
    }
}

static void mapGenAreasDivideP(NMap* Map, AListCompare* SortCallback, unsigned Percentage, int BlockSizeMin, int RoadSize, int Iterations)
{
    unsigned num = a_list_sizeGet(Map->areas) * Percentage / 100;

    mapGenAreasDivideN(
        Map, SortCallback, num, BlockSizeMin, RoadSize, Iterations);
}

static void mapGenAreasDiscardAroundEdge(NMap* Map)
{
    A_LIST_ITERATE(Map->areas, ZArea*, a) {
        if(a->x == 0 || a->y == 0
            || a->x + a->w == N_MAP_W || a->y + a->h == N_MAP_H) {

            z_area_free(a);
            A_LIST_REMOVE_CURRENT();
        }
    }
}

static void mapGenAreasDrawRoads(NMap* Map)
{
    A_LIST_ITERATE(Map->areas, ZArea*, a) {
        for(int x = a->x + a->w; x-- > a->x; ) {
            for(int r = a->road[Z_ROAD_UP].size; r--; ) {
                Map->tiles[a->y + r][x].id = U_TILE_ID_ROAD_H;
            }

            for(int r = a->road[Z_ROAD_DOWN].size; r--; ) {
                Map->tiles[a->y + a->h - 1 - r][x].id = U_TILE_ID_ROAD_H;
            }
        }

        for(int y = a->y + a->h; y-- > a->y; ) {
            for(int r = a->road[Z_ROAD_LEFT].size; r--; ) {
                Map->tiles[y][a->x + r].id = U_TILE_ID_ROAD_V;
            }

            for(int r = a->road[Z_ROAD_RIGHT].size; r--; ) {
                Map->tiles[y][a->x + a->w - 1 - r].id = U_TILE_ID_ROAD_V;
            }
        }
    }
}

static void mapGenAreasDrawBuildings(NMap* Map)
{
    A_LIST_ITERATE(Map->areas, ZArea*, a) {
        int startX = a->x + a->road[Z_ROAD_LEFT].size;
        int startY = a->y + a->road[Z_ROAD_UP].size;
        int endX = a->x + a->w - a->road[Z_ROAD_RIGHT].size;
        int endY = a->y + a->h - a->road[Z_ROAD_DOWN].size;

        for(int y = startY; y < endY; y++) {
            for(int x = startX; x < endX; x++) {
                Map->tiles[y][x].id = U_TILE_ID_BUILDING;
            }
        }
    }
}

static void mapGenAreasFloodFillVisit(NMap* Map, AList* Queue, int X, int Y)
{
    ZTile* tile = &Map->tiles[Y][X];

    if(!u_tile_flagsTest(tile->id, U_TILE_FLAG_NOMOVE)
        && !A_FLAG_TEST_ANY(tile->flags, Z_TILE_FLAG_VISITED)) {

        A_FLAG_SET(tile->flags, Z_TILE_FLAG_VISITED);
        a_list_addLast(Queue, z_coords_pack((AVectorInt){X, Y}));
    }
}

static void mapGenAreasFloodFill(NMap* Map)
{
    a_list_sort(Map->areas, z_area_cmpDistance);

    ZArea* centerMost = a_list_peek(Map->areas);
    AVectorInt start = {centerMost->x, centerMost->y};

    if(u_tile_flagsTest(Map->tiles[start.y][start.x].id, U_TILE_FLAG_NOMOVE)) {
        start.x += centerMost->w - 1;
        start.y += centerMost->h - 1;
    }

    AList* queue = a_list_new();

    mapGenAreasFloodFillVisit(Map, queue, start.x, start.y);

    while(!a_list_isEmpty(queue)) {
        AVectorInt coords = z_coords_unpack(a_list_removeFirst(queue));

        mapGenAreasFloodFillVisit(Map, queue, coords.x, coords.y - 1);
        mapGenAreasFloodFillVisit(Map, queue, coords.x, coords.y + 1);
        mapGenAreasFloodFillVisit(Map, queue, coords.x - 1, coords.y);
        mapGenAreasFloodFillVisit(Map, queue, coords.x + 1, coords.y);
    }

    for(int y = N_MAP_H; y--; ) {
        for(int x = N_MAP_W; x--; ) {
            if(!A_FLAG_TEST_ANY(Map->tiles[y][x].flags, Z_TILE_FLAG_VISITED)) {
                Map->tiles[y][x].id = U_TILE_ID_VOID;
            }
        }
    }

    a_list_free(queue);
}

static void mapGen(NMap* Map)
{
    ZArea* entireMap = z_area_new(0, 0, N_MAP_W, N_MAP_H);

    for(int r = 0; r < Z_ROAD_NUM; r++) {
        entireMap->road[r].size = 1;
    }

    a_list_addLast(Map->areas, entireMap);

    // Main shape
    mapGenAreasDivideN(Map, NULL, 1, 24, 4, 16);

    // Split into smaller blocks around the center
    mapGenAreasDivideP(Map, z_area_cmpDistance, 50, 8, 2, 8);
    mapGenAreasDivideP(Map, z_area_cmpDistance, 25, 4, 1, 8);

    // Clean map edges
    mapGenAreasDiscardAroundEdge(Map);

    // Split up the largest blocks
    mapGenAreasDivideN(Map, z_area_cmpSizeInv, 2, 8, 2, 2);

    // Split up the blocks farthest from center
    mapGenAreasDivideN(Map, z_area_cmpDistanceInv, 4, 8, 1, 3);

    mapGenAreasDrawRoads(Map);
    mapGenAreasFloodFill(Map);
    mapGenAreasDrawBuildings(Map);
}

void n_map_new(void)
{
    memset(&g_map, 0, sizeof(NMap));
    g_map.areas = a_list_new();

    mapGen(&g_map);
}

void n_map_free(void)
{
    a_list_freeEx(g_map.areas, (AFree*)z_area_free);
    g_map.areas = NULL;
}

void n_map_tick(void)
{
    //
}

void n_map_draw(void)
{
    #if Z_SHOW_MAP
        a_pixel_colorSetHex(0xaa8844);
        a_draw_fill();

        int offsetX = (a_screen_sizeGetWidth() - N_MAP_W) / 2;
        int offsetY = (a_screen_sizeGetHeight() - N_MAP_H) / 2;

        for(int y = 0; y < N_MAP_H; y++) {
            for(int x = 0; x < N_MAP_W; x++) {
                a_pixel_colorSetPixel(u_tile_colorGet(g_map.tiles[y][x].id));
                a_draw_pixel(offsetX + x, offsetY + y);
            }
        }

        return;
    #endif

    AVectorInt tileStart, tileEnd;
    AVectorInt screenStart;
    n_map_visibleGet(&tileStart, &tileEnd, &screenStart);

    for(int tileY = tileStart.y, screenY = screenStart.y;
        tileY < tileEnd.y;
        tileY++, screenY += Z_COORDS_PIXELS_PER_UNIT) {

        for(int tileX = tileStart.x, screenX = screenStart.x;
            tileX < tileEnd.x;
            tileX++, screenX += Z_COORDS_PIXELS_PER_UNIT) {

            a_sprite_blit(u_tile_spriteGet(g_map.tiles[tileY][tileX].id),
                          screenX,
                          screenY);
        }
    }
}

void n_map_visibleGet(AVectorInt* TileStart, AVectorInt* TileEnd, AVectorInt* ScreenStart)
{
    AVectorFix origin = n_camera_originGet();

    AVectorFix topLeftCoords = {
        origin.x - z_coords_pixelsToUnits(a_screen_sizeGetWidth() / 2),
        origin.y - z_coords_pixelsToUnits(a_screen_sizeGetHeight() / 2)
    };

    AVectorInt topLeftTile = a_vectorfix_toInt(topLeftCoords);
    AVectorInt topLeftPixels = z_coords_unitsToPixels(topLeftCoords);

    AVectorInt topLeftScreen = {
        0 - (topLeftPixels.x & (Z_COORDS_PIXELS_PER_UNIT - 1)),
        0 - (topLeftPixels.y & (Z_COORDS_PIXELS_PER_UNIT - 1))
    };

    if(topLeftCoords.x < 0) {
        topLeftScreen.x += -topLeftTile.x * Z_COORDS_PIXELS_PER_UNIT;
        topLeftTile.x = 0;
    }

    if(topLeftCoords.y < 0) {
        topLeftScreen.y += -topLeftTile.y * Z_COORDS_PIXELS_PER_UNIT;
        topLeftTile.y = 0;
    }

    AVectorInt tileEnd = {
        a_math_min(
            topLeftTile.x
                + (a_screen_sizeGetWidth() + (Z_COORDS_PIXELS_PER_UNIT - 1))
                    / Z_COORDS_PIXELS_PER_UNIT + 1,
            N_MAP_W),

        a_math_min(
            topLeftTile.y
                + (a_screen_sizeGetHeight() + (Z_COORDS_PIXELS_PER_UNIT - 1))
                    / Z_COORDS_PIXELS_PER_UNIT + 1,
            N_MAP_H)
    };

    if(TileStart) {
        *TileStart = topLeftTile;
    }

    if(TileEnd) {
        *TileEnd = tileEnd;
    }

    if(ScreenStart) {
        AVectorInt shake = n_camera_shakeGet();

        topLeftScreen.x += shake.x;
        topLeftScreen.y += shake.y;

        *ScreenStart = topLeftScreen;
    }
}
