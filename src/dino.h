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

#include "game.h"
#include "land.h"
#include "mover.h"

typedef struct DinoInstance DinoInstance;

extern DinoInstance* dino_new(LandInstance* Land, int NumLegs);
extern void dino_free(DinoInstance* Dino);

extern void dinos_tick(AList* Dinos);
extern void dinos_draw(AList* Dinos);

extern bool dino_collidesWith(DinoInstance* Dino, int X, int Y, int W, int H);
