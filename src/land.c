/*
    Copyright 2016 Alex Margarit

    This file is part of Rushed Dinosaur Dare.

    Rushed Dinosaur Dare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Rushed Dinosaur Dare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rushed Dinosaur Dare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "land.h"

typedef struct Slope {
    int width;
    int y_offset;
} Slope;

typedef struct LandModel {
    APixel color;
    Slope* slopes;
    int numSlopes;
    int maxDrift;
    int lineThickness;
} LandModel;

typedef struct LandColumn {
    int y;
} LandColumn;

#define COLUMN_EXTRA_BUFFER 128 // TODO: must accomodate widest possible object

struct LandInstance {
    LandModel* model;
    LandColumn* columns;
    int numColumns;
    AList* slopes;
    int initialY;
    int start_y;
    AFix x_offset;
    AFix speed;
};

static Slope g_groundSlopes[] = {
    {4, -1},
    {4, +1},
    {8, -1},
    {8, +1},
    {16, -1},
    {16, +1},
};

static Slope g_hillSlopes[] = {
    {1, -1},
    {1, +1},
    {2, -1},
    {2, +1},
};

static LandModel g_models[LAND_TYPE_NUM];

void land_init(void)
{
    g_models[LAND_TYPE_GROUND].color = a_pixel_make(255, 255, 255);
    g_models[LAND_TYPE_GROUND].slopes = g_groundSlopes;
    g_models[LAND_TYPE_GROUND].numSlopes = sizeof(g_groundSlopes) / sizeof(g_groundSlopes[0]);
    g_models[LAND_TYPE_GROUND].maxDrift = 4;
    g_models[LAND_TYPE_GROUND].lineThickness = 2;

    g_models[LAND_TYPE_HILLS].color = a_pixel_make(0x55, 0xff, 0xff);
    g_models[LAND_TYPE_HILLS].slopes = g_hillSlopes;
    g_models[LAND_TYPE_HILLS].numSlopes = sizeof(g_hillSlopes) / sizeof(g_hillSlopes[0]);
    g_models[LAND_TYPE_HILLS].maxDrift = 24;
    g_models[LAND_TYPE_HILLS].lineThickness = 1;
}

LandInstance* land_new(LandType Type, int StartY, AFix Speed)
{
    LandInstance* land = a_mem_malloc(sizeof(LandInstance));
    LandModel* model = &g_models[Type];

    land->model = model;
    land->numColumns = 2 * COLUMN_EXTRA_BUFFER + a_screen_width();
    land->columns = a_mem_malloc(land->numColumns * sizeof(LandColumn));
    land->slopes = a_list_new();
    land->initialY = StartY;
    land->start_y = StartY;
    land->x_offset = 0;
    land->speed = Speed;

    int y = StartY;
    int totalWidth = 0;

    while(totalWidth < land->numColumns) {
        Slope* s = &model->slopes[a_random_int(model->numSlopes)];
        a_list_addLast(land->slopes, s);

        for(int i = 0; i < s->width && totalWidth + i < land->numColumns; i++) {
            land->columns[totalWidth + i].y = y;
        }

        if(a_math_abs(y + s->y_offset - land->initialY) <= land->model->maxDrift) {
            y += s->y_offset;
        }

        totalWidth += s->width;
    }

    land_tick(land);

    return land;
}

void land_free(LandInstance* Land)
{
    free(Land->columns);
    a_list_free(Land->slopes);
    free(Land);
}

void land_tick(LandInstance* Land)
{
    int oldOffset = a_fix_fixtoi(Land->x_offset);
    Land->x_offset += Land->speed;

    int x = -oldOffset;
    int y = Land->start_y;

    A_LIST_ITERATE(Land->slopes, Slope*, s) {
        for(int i = 0; i < s->width; i++) {
            if(x >= 0 && x < Land->numColumns) {
                Land->columns[x].y = y;
            }

            if(++x == 2 && a_fix_fixtoi(Land->x_offset) > oldOffset) {
                Land->start_y = y;
            }
        }

        if(a_math_abs(y + s->y_offset - Land->initialY) <= Land->model->maxDrift) {
            y += s->y_offset;
        }

        if(x >= Land->numColumns) {
            break;
        }
    }

    Slope* firstSlope = a_list_getFirst(Land->slopes);

    if(a_fix_fixtoi(Land->x_offset) == firstSlope->width) {
        Land->x_offset = 0;
        a_list_removeFirst(Land->slopes);

        for(int sum = 0; sum < firstSlope->width; ) {
            Slope* s = &Land->model->slopes[a_random_int(Land->model->numSlopes)];
            sum += s->width;
            a_list_addLast(Land->slopes, s);
        }
    }
}

void land_draw(LandInstance* Land)
{
    a_pixel_setPixel(Land->model->color);

    for(int x = 0; x < a_screen_width(); x++) {
        for(int y = Land->model->lineThickness; y--; ) {
            a_pixel_put(x, Land->columns[COLUMN_EXTRA_BUFFER + x].y + y);
        }
    }
}

int land_getMidY(LandInstance* Land, int X)
{
    return (a_screen_height() + Land->columns[X].y) / 2;
}

int land_getTopY(LandInstance* Land, int X)
{
    return Land->columns[X].y;
}

int land_getNumColumns(LandInstance* Land)
{
    return Land->numColumns;
}

int land_getBufferOffset(LandInstance* Land)
{
    return COLUMN_EXTRA_BUFFER;
}

AFix land_getSpeed(LandInstance* Land)
{
    return Land->speed;
}
