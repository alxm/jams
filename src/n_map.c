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

#include "n_map.h"
#include "main.h"

void n_map_new(void)
{
    //
}

void n_map_free(void)
{
    //
}

void n_map_tick(void)
{
    //
}

static void drawTiles(FVecFix TopLeft)
{
    FVecFix start = {f_fix_floor(TopLeft.x), f_fix_floor(TopLeft.y)};
    FVecInt w = f_vecfix_toInt(start);
    FVecInt startScreen = n_cam_coordsToScreen(start);
    FVecInt screenSize = f_screen_sizeGet();

    for(int y = startScreen.y; y < screenSize.y; y += N_CAM_SCALE) {
        w.x = f_fix_toInt(start.x);

        for(int x = startScreen.x; x < screenSize.x; x += N_CAM_SCALE) {
            static const uint32_t hex[4] = {
                0x2277aa,
                0x2288aa,
                0x3377aa,
                0x338899,
            };

            if(w.x >= 0 && w.x < N_MAP_W
                && w.y >= 0 && w.y < N_MAP_H) {

                f_color_colorSetHex(hex[((w.x & 1) << 1) | (w.y & 1)]);
                f_draw_rectangle(x, y, N_CAM_SCALE, N_CAM_SCALE);
            }

            w.x++;
        }

        w.y++;
    }
}

static void drawGrid(FVecFix TopLeft)
{
    FVecFix start = {f_fix_ceiling(TopLeft.x), f_fix_ceiling(TopLeft.y)};
    FVecInt w = f_vecfix_toInt(start);
    FVecInt startScreen = n_cam_coordsToScreen(start);
    FVecInt screenSize = f_screen_sizeGet();

    f_color_colorSetHex(0x44aaaa);

    for(int x = startScreen.x; x < screenSize.x; x += N_CAM_SCALE) {
        if(w.x >= 0 && w.x <= N_MAP_W) {
            f_draw_linev(x, 0, screenSize.y - 1);
        }

        w.x++;
    }

    for(int y = startScreen.y; y < screenSize.y; y += N_CAM_SCALE) {
        if(w.y >= 0 && w.y <= N_MAP_H) {
            f_draw_lineh(0, screenSize.x - 1, y);
        }

        w.y++;
    }
}

void n_map_draw(void)
{
    f_color_blendSet(F_COLOR_BLEND_SOLID);
    f_color_colorSetHex(0x3388aa);
    f_draw_fill();

    FVecFix topLeft = n_cam_coordsFromScreen(0, 0);

    drawTiles(topLeft);
    drawGrid(topLeft);
}
