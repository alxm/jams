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

#define Z_MAP_W A_CONFIG_SCREEN_WIDTH
#define Z_MAP_H A_CONFIG_SCREEN_WIDTH

#define Z_BLOCK_SIZE_MULTIPLE 8
#define Z_BLOCK_SIZE_MIN (4 * Z_BLOCK_SIZE_MULTIPLE)

typedef struct {
    int x, y, w, h;
} ZArea;

typedef struct {
    int value;
} ZMapTile;

typedef struct {
    ZMapTile tiles[Z_MAP_H][Z_MAP_W];
} ZMap;

static ZMap g_map;

bool done = false;

static ZArea* z_area_new(int X, int Y, int W, int H)
{
    ZArea* a = a_mem_malloc(sizeof(ZArea));

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

static void z_map_generate(ZMap* Map)
{
    memset(Map, 0, sizeof(ZMap));

    AList* areas = a_list_new();
    ZArea* startArea = z_area_new(0, 0, Z_MAP_W, Z_MAP_H);

    a_list_addLast(areas, startArea);

    for(int iterations = 32; iterations--; ) {
        ZArea* areaOld = NULL;

        for(int tries = 8; tries--; ) {
            ZArea* a = a_list_getRandom(areas);

            if(a->w >= Z_BLOCK_SIZE_MIN || a->h >= Z_BLOCK_SIZE_MIN) {
                areaOld = a;
                break;
            }
        }

        if(areaOld == NULL) {
            A_LIST_ITERATE(areas, ZArea*, a) {
                if(a->w >= Z_BLOCK_SIZE_MIN || a->h >= Z_BLOCK_SIZE_MIN) {
                    areaOld = a;
                    break;
                }
            }
        }

        if(areaOld == NULL) {
            a_out_printf("BAIL: %d iterations left\n", iterations + 1);
            done = true;
            break;
        }

        ZArea* areaNew;
        bool splitAlongHeight = areaOld->w == areaOld->h
                                    ? a_random_int(2)
                                    : areaOld->w > areaOld->h;

        if(splitAlongHeight) {
            int smallest = a_math_max(Z_BLOCK_SIZE_MULTIPLE, areaOld->w / 4);
            int median = a_random_range(smallest, areaOld->w - smallest + 1);

            areaNew = z_area_new(areaOld->x + median,
                                 areaOld->y,
                                 areaOld->w - median,
                                 areaOld->h);

            areaOld->w = median;
        } else {
            int smallest = a_math_max(Z_BLOCK_SIZE_MULTIPLE, areaOld->h / 4);
            int median = a_random_range(smallest, areaOld->h - smallest + 1);

            areaNew = z_area_new(areaOld->x,
                                 areaOld->y + median,
                                 areaOld->w,
                                 areaOld->h - median);

            areaOld->h = median;
        }

        a_list_addLast(areas, areaNew);
    }

    A_LIST_ITERATE(areas, ZArea*, a) {
        int y = a->y + a->h;

        if(y < Z_MAP_H - 1) {
            for(int x = a->x + a->w; x-- > a->x; ) {
                Map->tiles[y - 3][x].value = 3;
                Map->tiles[y - 2][x].value = 3;
                Map->tiles[y - 1][x].value = 4;
                Map->tiles[y - 0][x].value = 4;
            }
        }

        int x = a->x + a->w;

        if(x < Z_MAP_W - 1) {
            for(int y = a->y + a->h; y-- > a->y; ) {
                Map->tiles[y][x - 3].value = 4;
                Map->tiles[y][x - 2].value = 4;
                Map->tiles[y][x - 1].value = 3;
                Map->tiles[y][x - 0].value = 3;
            }
        }
    }

    a_list_freeEx(areas, (AFree*)z_area_free);
}

static void z_map_draw(const ZMap* Map)
{
    const APixel colors[] = {
        a_pixel_fromHex(0xaaff88),
        a_pixel_fromHex(0xffb922),
        a_pixel_fromHex(0xff8822),
        a_pixel_fromHex(0xff6022),
        a_pixel_fromHex(0xff3722),
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
        if(!done && a_fps_ticksNth(A_CONFIG_FPS_RATE_TICK / 2)) {
            z_map_generate(&g_map);
        }
    }

    A_STATE_DRAW
    {
        z_map_draw(&g_map);
    }
}
