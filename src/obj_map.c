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

#define Z_BUILDING_MIN_W 2
#define Z_BUILDING_MIN_H 4

typedef enum {
    Z_TILE_FLAG_ROAD = A_FLAG_BIT(0),
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
    const ZArea* area;
    UTileId id;
    const UTileInstance* tile;
    ZTileFlags flags;
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

static inline int z_area_sizeGetWidthUsable(const ZArea* Area)
{
    return Area->w
            - Area->road[Z_ROAD_LEFT].size - Area->road[Z_ROAD_RIGHT].size;
}

static inline int z_area_sizeGetHeightUsable(const ZArea* Area)
{
    return Area->h
            - Area->road[Z_ROAD_UP].size - Area->road[Z_ROAD_DOWN].size;
}

static bool z_area_canDivide(const ZArea* Area, int BlockSizeMin, int RoadSize)
{
    return z_area_sizeGetWidthUsable(Area) >= 2 * BlockSizeMin + RoadSize
            || z_area_sizeGetHeightUsable(Area) >= 2 * BlockSizeMin + RoadSize;
}

static AList* z_area_subdivide(ZArea* StartArea, int BlockSizeMin, int RoadSize, int Iterations)
{
    AList* areas = a_list_new();

    a_list_addLast(areas, StartArea);

    while(Iterations--) {
        ZArea* areaOld = NULL;

        for(int tries = 8; tries--; ) {
            ZArea* a = a_list_getRandom(areas);

            if(z_area_canDivide(a, BlockSizeMin, RoadSize)) {
                areaOld = a;
                break;
            }
        }

        if(areaOld == NULL) {
            A_LIST_ITERATE(areas, ZArea*, a) {
                if(z_area_canDivide(a, BlockSizeMin, RoadSize)) {
                    areaOld = a;
                    break;
                }
            }
        }

        if(areaOld == NULL) {
            break;
        }

        ZArea* areaNew;
        int usableWidth = z_area_sizeGetWidthUsable(areaOld);
        int usableHeight = z_area_sizeGetHeightUsable(areaOld);
        bool splitAlongHeight = usableWidth == usableHeight
                                    ? a_random_int(2)
                                    : usableWidth > usableHeight;

        if(splitAlongHeight) {
            int min = areaOld->road[Z_ROAD_LEFT].size
                        + BlockSizeMin
                        + (RoadSize >> 1);
            int max = areaOld->w
                        - areaOld->road[Z_ROAD_RIGHT].size
                        - BlockSizeMin
                        - (RoadSize >> 1);

            int median = a_random_range(min, max + 1);

            areaNew = z_area_new(areaOld->x + median,
                                 areaOld->y,
                                 areaOld->w - median,
                                 areaOld->h);

            areaOld->w = median;
            memcpy(areaNew->road, areaOld->road, sizeof(areaOld->road));

            if(areaOld->w - areaOld->road[Z_ROAD_LEFT].size
                > areaNew->w - areaNew->road[Z_ROAD_RIGHT].size) {

                areaOld->road[Z_ROAD_RIGHT].size = RoadSize - (RoadSize >> 1);
                areaNew->road[Z_ROAD_LEFT].size = RoadSize >> 1;
            } else {
                areaOld->road[Z_ROAD_RIGHT].size = RoadSize >> 1;
                areaNew->road[Z_ROAD_LEFT].size = RoadSize - (RoadSize >> 1);
            }
        } else {
            int min = areaOld->road[Z_ROAD_UP].size
                        + BlockSizeMin
                        + (RoadSize >> 1);
            int max = areaOld->h
                        - areaOld->road[Z_ROAD_DOWN].size
                        - BlockSizeMin
                        - (RoadSize >> 1);

            int median = a_random_range(min, max + 1);

            areaNew = z_area_new(areaOld->x,
                                 areaOld->y + median,
                                 areaOld->w,
                                 areaOld->h - median);

            areaOld->h = median;
            memcpy(areaNew->road, areaOld->road, sizeof(areaOld->road));

            if(areaOld->h - areaOld->road[Z_ROAD_UP].size
                > areaNew->h - areaNew->road[Z_ROAD_DOWN].size) {

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

            if(a_list_isEmpty(Map->areas)) {
                A_FATAL("Discard edges: Removed last area");
            }
        }
    }
}

static void mapGenAreasShrinkToUsableSize(NMap* Map)
{
    A_LIST_ITERATE(Map->areas, ZArea*, a) {
        a->x += a->road[Z_ROAD_LEFT].size;
        a->y += a->road[Z_ROAD_UP].size;
        a->w -= a->road[Z_ROAD_LEFT].size + a->road[Z_ROAD_RIGHT].size;
        a->h -= a->road[Z_ROAD_UP].size + a->road[Z_ROAD_DOWN].size;
    }
}

static void mapGenAreasLinkTiles(NMap* Map)
{
    A_LIST_ITERATE(Map->areas, ZArea*, a) {
        for(int y = a->y; y < a->y + a->h; y++) {
            for(int x = a->x; x < a->x + a->w; x++) {
                Map->tiles[y][x].area = a;
            }
        }
    }
}

static void mapGenAreasPutRoads(NMap* Map)
{
    A_LIST_ITERATE(Map->areas, ZArea*, a) {
        for(int x = a->x; x < a->x + a->w; x++) {
            for(int r = a->road[Z_ROAD_UP].size; r--; ) {
                ZTile* t = &Map->tiles[a->y + r][x];

                t->id = U_TILE_ID_ROAD_H;
                A_FLAG_SET(t->flags, Z_TILE_FLAG_ROAD);
            }

            for(int r = a->road[Z_ROAD_DOWN].size; r--; ) {
                ZTile* t = &Map->tiles[a->y + a->h - 1 - r][x];

                t->id = U_TILE_ID_ROAD_H;
                A_FLAG_SET(t->flags, Z_TILE_FLAG_ROAD);
            }
        }

        for(int y = a->y; y < a->y + a->h; y++) {
            for(int r = a->road[Z_ROAD_LEFT].size; r--; ) {
                ZTile* t = &Map->tiles[y][a->x + r];

                t->id = U_TILE_ID_ROAD_V;
                A_FLAG_SET(t->flags, Z_TILE_FLAG_ROAD);
            }

            for(int r = a->road[Z_ROAD_RIGHT].size; r--; ) {
                ZTile* t = &Map->tiles[y][a->x + a->w - 1 - r];

                t->id = U_TILE_ID_ROAD_V;
                A_FLAG_SET(t->flags, Z_TILE_FLAG_ROAD);
            }
        }
    }
}

static inline void put(NMap* Map, int X, int Y, UTileId Tile)
{
    Map->tiles[Y][X].id = Tile;
}

static void putGrid(NMap* Map, int X, int Y, int W, int H, UTileId TileBase)
{
    put(Map, X, Y, TileBase);
    put(Map, X, Y + H - 1, TileBase + 6);

    for(int x = X + 1; x < X + W - 1; x++) {
        put(Map, x, Y, TileBase + 1);
        put(Map, x, Y + H - 1, TileBase + 7);
    }

    for(int y = Y + 1; y < Y + H - 1; y++) {
        put(Map, X, y, TileBase + 3);
        put(Map, X + W - 1, y, TileBase + 5);

        for(int x = X + 1; x < X + W - 1; x++) {
            put(Map, x, y, TileBase + 4);
        }
    }

    put(Map, X + W - 1, Y, TileBase + 2);
    put(Map, X + W - 1, Y + H - 1, TileBase + 8);
}

static void mapGenAreaPutForest(NMap* Map, ZArea* Area)
{
    putGrid(Map, Area->x, Area->y, Area->w, Area->h, U_TILE_ID_MISC_TREE_1);
}

static void mapGenAreaPutSidewalks(NMap* Map, ZArea* Area)
{
    bool sides = Area->w >= Z_BUILDING_MIN_W + 2;
    bool bottom = Area->h >= Z_BUILDING_MIN_H + 1;

    if(sides) {
        put(Map, Area->x, Area->y, U_TILE_ID_SIDEWALK_1);
        put(Map, Area->x + Area->w - 1, Area->y, U_TILE_ID_SIDEWALK_3);

        for(int y = Area->y + 1; y < Area->y + Area->h - 1; y++) {
            put(Map, Area->x, y, U_TILE_ID_SIDEWALK_4);
            put(Map, Area->x + Area->w - 1, y, U_TILE_ID_SIDEWALK_6);
        }
    }

    if(bottom) {
        for(int x = Area->x + 1; x < Area->x + Area->w - 1; x++) {
            put(Map, x, Area->y + Area->h - 1, U_TILE_ID_SIDEWALK_8);
        }
    }

    if(sides || bottom) {
        put(Map, Area->x, Area->y + Area->h - 1, U_TILE_ID_SIDEWALK_7);
        put(Map,
            Area->x + Area->w - 1,
            Area->y + Area->h - 1,
            U_TILE_ID_SIDEWALK_9);

        if(sides) {
            Area->x += 1;
            Area->w -= 2;
        }

        if(bottom) {
            Area->h -= 1;
        }
    }
}

static void mapGenAreaPutBuilding(NMap* Map, ZArea* Area)
{
    int roofLen = a_math_max(2, Area->h / 4);
    int frontLen = Area->h - roofLen;

    putGrid(Map, Area->x, Area->y, Area->w, roofLen, U_TILE_ID_B0_ROOF_1);
    putGrid(Map,
            Area->x,
            Area->y + roofLen,
            Area->w,
            frontLen,
            U_TILE_ID_B0_FRONT_1);

    int doorX = a_random_int(Area->w);

    put(Map,
        Area->x + doorX,
        Area->y + Area->h - 1,
        doorX == 0 ? U_TILE_ID_B0_DOOR_7
            : doorX == Area->w - 1 ? U_TILE_ID_B0_DOOR_9 : U_TILE_ID_B0_DOOR_8);

    if(frontLen > 1) {
        for(int y = Area->y + roofLen; y < Area->y + Area->h - 1; y++) {
            for(int x = Area->x; x < Area->x + Area->w; x++) {
                if(a_random_chance(2, 3)) {
                    if(x == Area->x) {
                        put(Map, x, y, U_TILE_ID_B0_WINDOW_4);
                    } else if(x == Area->x + Area->w - 1) {
                        put(Map, x, y, U_TILE_ID_B0_WINDOW_6);
                    } else {
                        put(Map, x, y, U_TILE_ID_B0_WINDOW_5);
                    }
                }
            }
        }
    }
}

static void mapGenAreasPutBlocks(NMap* Map)
{
    A_LIST_ITERATE(Map->areas, ZArea*, a) {
        if(a_random_chance(9, 10)
            && a->w >= Z_BUILDING_MIN_W && a->h >= Z_BUILDING_MIN_H) {

            mapGenAreaPutSidewalks(Map, a);
            mapGenAreaPutBuilding(Map, a);
        } else {
            mapGenAreaPutForest(Map, a);
        }
    }
}

static void mapGenAreasFloodFillVisit(NMap* Map, AList* Queue, int X, int Y)
{
    ZTile* tile = &Map->tiles[Y][X];

    if(A_FLAG_TEST_ANY(tile->flags, Z_TILE_FLAG_ROAD)
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

    if(!A_FLAG_TEST_ANY(Map->tiles[start.y][start.x].flags, Z_TILE_FLAG_ROAD)) {
        AVectorInt corners[3] = {
            {start.x + centerMost->w - 1, start.y},
            {start.x + centerMost->w - 1, start.y + centerMost->h - 1},
            {start.x, start.y + centerMost->h - 1},
        };

        for(int c = A_ARRAY_LEN(corners); c--; ) {
            start = corners[c];

            if(A_FLAG_TEST_ANY(
                Map->tiles[start.y][start.x].flags, Z_TILE_FLAG_ROAD)) {

                break;
            }
        }
    }

    if(!A_FLAG_TEST_ANY(Map->tiles[start.y][start.x].flags, Z_TILE_FLAG_ROAD)) {
        A_FATAL("Flood fill: Could not find an origin");
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

    a_list_free(queue);

    A_LIST_ITERATE(Map->areas, ZArea*, a) {
        AVectorInt corners[4] = {
            {a->x, a->y},
            {a->x + a->w - 1, a->y},
            {a->x + a->w - 1, a->y + a->h - 1},
            {a->x, a->y + a->h - 1},
        };

        for(int c = A_ARRAY_LEN(corners); c--; ) {
            const ZTile* t = &Map->tiles[corners[c].y][corners[c].x];

            if(A_FLAG_TEST_ANY(t->flags, Z_TILE_FLAG_ROAD)
                && !A_FLAG_TEST_ANY(t->flags, Z_TILE_FLAG_VISITED)) {

                for(int y = a->y; y < a->y + a->h; y++) {
                    for(int x = a->x; x < a->x + a->w; x++) {
                        Map->tiles[y][x].id = U_TILE_ID_MISC_VOID;
                    }
                }

                z_area_free(a);
                A_LIST_REMOVE_CURRENT();

                if(a_list_isEmpty(Map->areas)) {
                    A_FATAL("Flood fill: Removed last area");
                }

                break;
            }
        }
    }
}

static void mapGenTilesGetInstances(NMap* Map)
{
    for(int y = N_MAP_H; y--; ) {
        for(int x = N_MAP_W; x--; ) {
            Map->tiles[y][x].tile = u_tile_get(Map->tiles[y][x].id);
        }
    }
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
    mapGenAreasDivideP(Map, z_area_cmpDistance, 25, 6, 1, 8);

    // Clean map edges
    mapGenAreasDiscardAroundEdge(Map);

    // Split up the largest blocks
    mapGenAreasDivideP(Map, z_area_cmpSizeInv, 20, 8, 2, 2);

    // Split up the blocks farthest from center
    mapGenAreasDivideP(Map, z_area_cmpDistanceInv, 20, 4, 1, 3);

    // Each tile knows what block it's in
    mapGenAreasLinkTiles(Map);

    // Put road tiles and clear unreachable ones
    mapGenAreasPutRoads(Map);
    mapGenAreasFloodFill(Map);

    // Shrink blocks away from streets and populate each one
    mapGenAreasShrinkToUsableSize(Map);
    mapGenAreasPutBlocks(Map);

    // Get tile sprites
    mapGenTilesGetInstances(Map);
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

            a_sprite_blit(u_tile_spriteGet(g_map.tiles[tileY][tileX].tile),
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
