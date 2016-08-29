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

typedef struct MovingObject MovingObject;
typedef void (*MoverSetLegOffsets)(int NumLegs, int* LegX, int* LegY);

#include "land.h"

extern MovingObject* mover_new(LandInstance* Land, int StartX, int StartY, int NumLegs, MoverSetLegOffsets LegOffsetsCallback);
extern void mover_free(MovingObject* Mover);
extern void mover_addLegFrames(MovingObject* Mover, ASpriteFrames* LegFrames);
extern void mover_getCoords(MovingObject* Mover, int* X, int* Y);
extern void mover_moveX(MovingObject* Mover, AFix Dx);
extern void mover_jump(MovingObject* Mover);
extern void mover_tick(MovingObject* Mover);
extern void mover_drawLegs(MovingObject* Mover);
extern void mover_pauseFrames(MovingObject* Mover);
extern void mover_resumeFrames(MovingObject* Mover);
extern void mover_setFramesSpeed(MovingObject* Mover, unsigned int CallsToNextFrame);
