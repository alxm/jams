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

#include "obj_camera.h"

#include "obj_map.h"
#include "util_coords.h"

#define Z_INERTIA_SHIFT 2

typedef struct {
    AVectorFix coords;
    AVectorInt shake;
    ATimer* shakeTimer;
} NCamera;

static NCamera g_camera;

void n_camera_new(AVectorInt TileCoords)
{
    g_camera.coords = a_vectorint_toFix(TileCoords);
    g_camera.shake = (AVectorInt){0, 0};

    if(g_camera.shakeTimer == NULL) {
        g_camera.shakeTimer = a_timer_new(A_TIMER_MS, 0, false);
    } else {
        a_timer_stop(g_camera.shakeTimer);
    }
}

void n_camera_free(void)
{
    a_timer_free(g_camera.shakeTimer);
    g_camera.shakeTimer = NULL;
}

void n_camera_tick(AVectorInt TileCoords)
{
    AVectorFix target = a_vectorint_toFix(TileCoords);
    target.x += A_FIX_ONE / 2;
    target.y += A_FIX_ONE / 2;

    AFix halfScrWUnits = z_coords_pixelsToUnits(a_screen_sizeGetWidth() / 2);
    AFix halfScrHUnits = z_coords_pixelsToUnits(a_screen_sizeGetHeight() / 2);

    g_camera.coords.x = a_math_clamp(
        (target.x >> Z_INERTIA_SHIFT)
            + (g_camera.coords.x - (g_camera.coords.x >> Z_INERTIA_SHIFT)),
        halfScrWUnits,
        a_fix_fromInt(N_MAP_W) - halfScrWUnits);

    g_camera.coords.y = a_math_clamp(
        (target.y >> Z_INERTIA_SHIFT)
            + (g_camera.coords.y - (g_camera.coords.y >> Z_INERTIA_SHIFT)),
        halfScrHUnits,
        a_fix_fromInt(N_MAP_H) - halfScrHUnits);

    if(a_timer_isRunning(g_camera.shakeTimer)) {
        g_camera.shake = (AVectorInt){a_random_range(-1, 2),
                                      a_random_range(-1, 2)};
    } else {
        g_camera.shake = (AVectorInt){0, 0};
    }
}

AVectorFix n_camera_originGet(void)
{
    return g_camera.coords;
}

AVectorInt n_camera_coordsToScreen(AVectorFix WorldCoords)
{
    AVectorFix relative = {WorldCoords.x - g_camera.coords.x,
                           WorldCoords.y - g_camera.coords.y};

    AVectorInt coords = z_coords_unitsToPixels(relative);

    coords.x += a_screen_sizeGetWidth() / 2 + g_camera.shake.x;
    coords.y += a_screen_sizeGetHeight() / 2 + g_camera.shake.y;

    return coords;
}

AVectorInt n_camera_shakeGet(void)
{
    return g_camera.shake;
}

void n_camera_shakeSet(unsigned Ms)
{
    a_timer_periodSet(g_camera.shakeTimer, Ms);
    a_timer_start(g_camera.shakeTimer);
}
