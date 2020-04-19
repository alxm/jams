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

#include "n_cam.h"
#include "main.h"

#define N_CAM_ZOOM 256

typedef struct {
    FVecFix coords;
    FVecInt shake;
    FTimer* timer;
    int zoom;
} NCam;

static NCam g_cam;

void n_cam_new(void)
{
    g_cam.shake = (FVecInt){0, 0};
    g_cam.zoom = N_CAM_ZOOM; // 1 unit is this many screen pixels

    if(g_cam.timer == NULL) {
        g_cam.timer = f_timer_new(F_TIMER_MS, 0, false);
    }

    f_timer_runStop(g_cam.timer);
}

void n_cam_free(void)
{
    f_timer_free(g_cam.timer);
}

void n_cam_tick(FVecFix Origin)
{
    g_cam.coords = Origin;

    if(f_timer_runGet(g_cam.timer)) {
        g_cam.shake = (FVecInt){f_random_range(-1, 2), f_random_range(-1, 2)};
    } else {
        g_cam.shake = (FVecInt){0, 0};
    }

    g_cam.zoom = N_CAM_ZOOM
                    + f_fix_toInt(f_fps_ticksSin(1, 3, 0) * N_CAM_ZOOM / 8);
}

FVecFix n_cam_coordsGetOrigin(void)
{
    return g_cam.coords;
}

FVecFix n_cam_coordsFromScreen(int X, int Y)
{
    FVecInt screen = f_screen_sizeGet();

    return (FVecFix){
        g_cam.coords.x - f_fix_fromInt(screen.x / 2 - X) / g_cam.zoom,
        g_cam.coords.y - f_fix_fromInt(screen.y / 2 - Y) / g_cam.zoom
    };
}

FVecInt n_cam_coordsToScreen(FVecFix Coords)
{
    FVecInt screen = f_screen_sizeGet();
    FVecFix relative = {Coords.x - g_cam.coords.x, Coords.y - g_cam.coords.y};
    FVecInt coords = {
        screen.x / 2 + f_fix_toInt(relative.x * g_cam.zoom) + g_cam.shake.x,
        screen.y / 2 + f_fix_toInt(relative.y * g_cam.zoom) + g_cam.shake.y
    };

    return coords;
}

int n_cam_zoomGet(void)
{
    return g_cam.zoom;
}

FVecInt n_cam_shakeGet(void)
{
    return g_cam.shake;
}

void n_cam_shakeSet(unsigned Ms)
{
    f_timer_periodSet(g_cam.timer, Ms);
    f_timer_runStart(g_cam.timer);
}
