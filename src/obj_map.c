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

enum {
    Z__COORDS_CAN_PACK_IN_A_VOID_PTR = 1 / (sizeof(void*) >= 4),
    Z__COORDS_CAN_PACK_IN_A_PTRDIFF = 1 / (sizeof(ptrdiff_t) >= 4),
};

static inline void* z_coords_pack(AVectorInt Coords)
{
    return (void*)(ptrdiff_t)
        (((Coords.y & 0xffff) << 16) | (Coords.x & 0xffff));
}

static inline AVectorInt z_coords_unpack(void* Point)
{
    return (AVectorInt){(ptrdiff_t)Point & 0xffff,
                        ((ptrdiff_t)Point >> 16) & 0xffff};
}

#define Z_BLOCK_SIZE_MULTIPLE 16

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
} ZMapTile;

typedef struct {
    ZMapTile tiles[N_MAP_H][N_MAP_W];
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

static AList* z_area_subdivide(ZArea* StartArea, int Iterations, int BlockSizeMin, int RoadSize)
{
    AList* areas = a_list_new();

    a_list_addLast(areas, StartArea);

    while(Iterations--) {
        ZArea* areaOld = NULL;

        for(int tries = 8; tries--; ) {
            ZArea* a = a_list_getRandom(areas);

            if(a->w >= BlockSizeMin || a->h >= BlockSizeMin) {
                areaOld = a;
                break;
            }
        }

        if(areaOld == NULL) {
            A_LIST_ITERATE(areas, ZArea*, a) {
                if(a->w >= BlockSizeMin || a->h >= BlockSizeMin) {
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
                                    ? a_random_int(2)
                                    : areaOld->w > areaOld->h;

        if(splitAlongHeight) {
            int smallest = a_math_max(BlockSizeMin / 2, areaOld->w / 4);
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
            int smallest = a_math_max(BlockSizeMin / 2, areaOld->h / 4);
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

static int areasCmpSize(void* ItemA, void* ItemB)
{
    return z_area_surfaceGet(ItemB) - z_area_surfaceGet(ItemA);
}

static int areasCmpDistance(void* ItemA, void* ItemB)
{
    return z_area_distanceGetSq(ItemA) - z_area_distanceGetSq(ItemB);
}

static int areasCmpDistanceInv(void* ItemA, void* ItemB)
{
    return z_area_distanceGetSq(ItemB) - z_area_distanceGetSq(ItemA);
}

static void areasDivide(AList* Areas, AListCompare* SortFunction, unsigned Percentage, int Iterations, int BlockSizeMin, int RoadSize)
{
    a_list_sort(Areas, SortFunction);

    for(unsigned i = a_list_sizeGet(Areas) * Percentage / 100; i--; ) {
        AList* subAreas = z_area_subdivide(a_list_pop(Areas),
                                           Iterations,
                                           BlockSizeMin,
                                           RoadSize);

        a_list_appendMove(Areas, subAreas);
        a_list_free(subAreas);
    }
}

static void z_map_generate(NMap* Map)
{
    memset(Map, 0, sizeof(NMap));

    ZArea* entireArea = z_area_new(0, 0, N_MAP_W, N_MAP_H);

    for(int r = 0; r < Z_ROAD_NUM; r++) {
        entireArea->road[r].size = 4;
    }

    AList* areas = z_area_subdivide(entireArea,
                                    32,
                                    4 * Z_BLOCK_SIZE_MULTIPLE,
                                    8);

    areasDivide(areas,
                areasCmpDistance,
                20,
                8,
                2 * Z_BLOCK_SIZE_MULTIPLE,
                4);

    areasDivide(areas,
                areasCmpDistance,
                12,
                8,
                1 * Z_BLOCK_SIZE_MULTIPLE,
                2);

    areasDivide(areas,
                areasCmpSize,
                6,
                2,
                1 * Z_BLOCK_SIZE_MULTIPLE,
                2);

    A_LIST_ITERATE(areas, ZArea*, a) {
        if(a->x == 0 || a->y == 0
            || a->x + a->w == N_MAP_W || a->y + a->h == N_MAP_H) {

            z_area_free(a);
            A_LIST_REMOVE_CURRENT();
        }
    }

    areasDivide(areas,
                areasCmpDistanceInv,
                20,
                3,
                1 * Z_BLOCK_SIZE_MULTIPLE,
                1);

    A_LIST_ITERATE(areas, ZArea*, a) {
        for(int x = a->x + a->w; x-- > a->x; ) {
            for(int r = a->road[Z_ROAD_UP].size; r--; ) {
                Map->tiles[a->y + r][x].id = 1;
            }

            for(int r = a->road[Z_ROAD_DOWN].size; r--; ) {
                Map->tiles[a->y + a->h - 1 - r][x].id = 1;
            }
        }

        for(int y = a->y + a->h; y-- > a->y; ) {
            for(int r = a->road[Z_ROAD_LEFT].size; r--; ) {
                Map->tiles[y][a->x + r].id = 1;
            }

            for(int r = a->road[Z_ROAD_RIGHT].size; r--; ) {
                Map->tiles[y][a->x + a->w - 1 - r].id = 1;
            }
        }
    }

    a_list_sort(areas, areasCmpDistance);

    ZArea* centerMost = a_list_peek(areas);
    AVectorInt origin = {centerMost->x, centerMost->y};
    AList* queue = a_list_new();

    #define Z_ENQUEUE(Base, X, Y) \
        if(Map->tiles[Base.y + Y][Base.x + X].id == 1) { \
            Map->tiles[Base.y + Y][Base.x + X].id = 2; \
            a_list_addLast(queue, \
                           z_coords_pack( \
                            (AVectorInt){Base.x + X, Base.y + Y})); \
        }

    Z_ENQUEUE(origin, 0, 0);

    while(!a_list_isEmpty(queue)) {
        AVectorInt coords = z_coords_unpack(a_list_removeFirst(queue));

        Z_ENQUEUE(coords, 0, -1);
        Z_ENQUEUE(coords, 0, 1);
        Z_ENQUEUE(coords, -1, 0);
        Z_ENQUEUE(coords, 1, 0);
    }

    for(int y = N_MAP_H; y--; ) {
        for(int x = N_MAP_W; x--; ) {
            if(Map->tiles[y][x].id > 0) {
                Map->tiles[y][x].id--;
            }
        }
    }

    a_list_free(queue);
    a_list_freeEx(areas, (AFree*)z_area_free);
}

void n_map_new(void)
{
    z_map_generate(&g_map);
}

void n_map_tick(void)
{
    //
}

void n_map_draw(void)
{
    a_pixel_colorSetHex(0x886644);
    a_draw_fill();

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

    #define Z_X_TILES \
        ((a_screen_sizeGetWidth() + (Z_COORDS_PIXELS_PER_UNIT - 1)) \
            / Z_COORDS_PIXELS_PER_UNIT + 1)

    #define Z_Y_TILES \
        ((a_screen_sizeGetHeight() + (Z_COORDS_PIXELS_PER_UNIT - 1)) \
            / Z_COORDS_PIXELS_PER_UNIT + 1)

    AVectorInt tileEnd = {
        a_math_min(topLeftTile.x + Z_X_TILES, N_MAP_W),
        a_math_min(topLeftTile.y + Z_Y_TILES, N_MAP_H)
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
