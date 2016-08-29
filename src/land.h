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

#pragma once

#include <a2x.h>

typedef enum LandType {
    LAND_TYPE_GROUND,
    LAND_TYPE_HILLS,
    LAND_TYPE_NUM
} LandType;

typedef struct LandInstance LandInstance;

extern void land_init(void);

extern LandInstance* land_new(LandType Type, int StartY, AFix Speed);
extern void land_free(LandInstance* Land);

extern void land_tick(LandInstance* Land);
extern void land_draw(LandInstance* Land);

extern int land_getTopY(LandInstance* Land, int X);
extern int land_getMidY(LandInstance* Land, int X);
extern int land_getNumColumns(LandInstance* Land);
extern int land_getBufferOffset(LandInstance* Land);
extern AFix land_getSpeed(LandInstance* Land);
