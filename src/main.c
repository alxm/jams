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

#include <a2x.h>

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

#define Z_MAP_W A_CONFIG_SCREEN_WIDTH
#define Z_MAP_H A_CONFIG_SCREEN_WIDTH

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
    int value;
} ZMapTile;

typedef struct {
    ZMapTile tiles[Z_MAP_H][Z_MAP_W];
} ZMap;

static ZMap g_map;

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

static inline int distanceFromCenterSq(const ZArea* Area)
{
    int x = Area->x + Area->w / 2 - Z_MAP_W / 2;
    int y = Area->y + Area->h / 2 - Z_MAP_H / 2;

    return x * x + y * y;
}

static inline int surfaceArea(const ZArea* Area)
{
    return Area->w * Area->h;
}

static int z_area_sortByDistance(void* ItemA, void* ItemB)
{
    return distanceFromCenterSq(ItemA) - distanceFromCenterSq(ItemB);
}

static int z_area_sortByDistanceInv(void* ItemA, void* ItemB)
{
    return distanceFromCenterSq(ItemB) - distanceFromCenterSq(ItemA);
}

static int z_area_sortBySize(void* ItemA, void* ItemB)
{
    return surfaceArea(ItemB) - surfaceArea(ItemA);
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

static void z_map_generate(ZMap* Map)
{
    memset(Map, 0, sizeof(ZMap));

    ZArea* entireArea = z_area_new(0, 0, Z_MAP_W, Z_MAP_H);

    for(int r = 0; r < Z_ROAD_NUM; r++) {
        entireArea->road[r].size = 4;
    }

    AList* areas = z_area_subdivide(entireArea,
                                    32,
                                    4 * Z_BLOCK_SIZE_MULTIPLE,
                                    8);

    areasDivide(areas,
                z_area_sortByDistance,
                20,
                8,
                2 * Z_BLOCK_SIZE_MULTIPLE,
                4);

    areasDivide(areas,
                z_area_sortByDistance,
                12,
                8,
                1 * Z_BLOCK_SIZE_MULTIPLE,
                2);

    areasDivide(areas,
                z_area_sortBySize,
                6,
                2,
                1 * Z_BLOCK_SIZE_MULTIPLE,
                2);

    A_LIST_ITERATE(areas, ZArea*, a) {
        if(a->x == 0 || a->y == 0
            || a->x + a->w == Z_MAP_W || a->y + a->h == Z_MAP_H) {

            z_area_free(a);
            A_LIST_REMOVE_CURRENT();
        }
    }

    areasDivide(areas,
                z_area_sortByDistanceInv,
                20,
                3,
                1 * Z_BLOCK_SIZE_MULTIPLE,
                1);

    A_LIST_ITERATE(areas, ZArea*, a) {
        for(int x = a->x + a->w; x-- > a->x; ) {
            for(int r = a->road[Z_ROAD_UP].size; r--; ) {
                Map->tiles[a->y + r][x].value = 1;
            }

            for(int r = a->road[Z_ROAD_DOWN].size; r--; ) {
                Map->tiles[a->y + a->h - 1 - r][x].value = 1;
            }
        }

        for(int y = a->y + a->h; y-- > a->y; ) {
            for(int r = a->road[Z_ROAD_LEFT].size; r--; ) {
                Map->tiles[y][a->x + r].value = 1;
            }

            for(int r = a->road[Z_ROAD_RIGHT].size; r--; ) {
                Map->tiles[y][a->x + a->w - 1 - r].value = 1;
            }
        }
    }

    a_list_sort(areas, z_area_sortByDistance);

    ZArea* centerMost = a_list_peek(areas);
    AVectorInt origin = {centerMost->x, centerMost->y};
    AList* queue = a_list_new();

    #define Z_ENQUEUE(Base, X, Y) \
        if(Map->tiles[Base.y + Y][Base.x + X].value == 1) { \
            Map->tiles[Base.y + Y][Base.x + X].value = 2; \
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

    for(int y = Z_MAP_H; y--; ) {
        for(int x = Z_MAP_W; x--; ) {
            switch(Map->tiles[y][x].value) {
                case 1:
                    Map->tiles[y][x].value = 0;
                    break;

                case 2:
                    Map->tiles[y][x].value = 1;
                    break;
            }
        }
    }

    a_list_free(queue);
    a_list_freeEx(areas, (AFree*)z_area_free);
}

static void z_map_draw(const ZMap* Map)
{
    const APixel colors[] = {
        a_pixel_fromHex(0x112244),
        a_pixel_fromHex(0xddb040),
        a_pixel_fromHex(0xdd8640),
        a_pixel_fromHex(0xdd6140),
    };

    for(int y = Z_MAP_H; y--; ) {
        for(int x = Z_MAP_W; x--; ) {
            int value = Map->tiles[y][x].value;
            a_pixel_colorSetPixel(colors[value]);
            a_draw_pixel(x, y);
        }
    }
}

typedef enum {
    Z_STATE_MAP,
    Z_STATE_NUM
} ZStateId;

A_STATE(s_map);

A_MAIN
{
    a_state_init(Z_STATE_NUM);
    a_state_new(Z_STATE_MAP, s_map, "City Map");

    a_state_push(Z_STATE_MAP);
}

A_STATE(s_map)
{
    A_STATE_INIT
    {
        z_map_generate(&g_map);
    }

    A_STATE_TICK
    {
        if(a_fps_ticksNth(A_CONFIG_FPS_RATE_TICK / 2)) {
            z_map_generate(&g_map);
        }
    }

    A_STATE_DRAW
    {
        z_map_draw(&g_map);
    }
}
