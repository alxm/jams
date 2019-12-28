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

typedef struct BuggyInstance BuggyInstance;

#include "dino.h"
#include "land.h"
#include "load.h"
#include "mover.h"

extern BuggyInstance* buggy_new(LandInstance* Land);
extern void buggy_free(BuggyInstance* Buggy);

extern void buggy_tick(BuggyInstance* Buggy);
extern void buggy_draw(BuggyInstance* Buggy);

extern int buggy_getX(BuggyInstance* Buggy);

extern void buggy_moveLeft(BuggyInstance* Buggy);
extern void buggy_moveRight(BuggyInstance* Buggy);
extern void buggy_jump(BuggyInstance* Buggy);
extern void buggy_shoot(BuggyInstance* Buggy);
