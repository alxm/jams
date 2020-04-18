/*
    Copyright 2020 Alex Margarit <alex@alxm.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <faur.h>

#define N_CAM_SCALE 32

extern void n_cam_new(void);
extern void n_cam_free(void);

extern void n_cam_tick(FVecFix Origin);

extern FVecFix n_cam_coordsGetOrigin(void);
extern FVecInt n_cam_coordsToScreen(FVecFix Coords);

extern FVecInt n_cam_shakeGet(void);
extern void n_cam_shakeSet(unsigned Ms);
