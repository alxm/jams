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

#include "n_hud.h"
#include "main.h"

static void drawLife(uint32_t Color, unsigned OffsetAngle, int OffsetX, int OffsetY)
{
    FVecInt screen = f_screen_sizeGet();
    int wmax = screen.x - 32;
    int hmax = screen.y / 16;

    int w = wmax * n_game.player->life / n_game.player->type->lifeFull
                + f_fix_toInt(
                    f_fps_ticksSin(1, 1, OffsetAngle + F_DEG_022_INT) * 4);
    int h = hmax + f_fix_toInt(f_fps_ticksSin(1, 1, OffsetAngle) * 4);

    f_color_colorSetHex(Color);
    f_color_blendSet(F_COLOR_BLEND_ALPHA_75);

    f_draw_rectangle(16 + (wmax - w) / 2 + OffsetX,
                     16 + (hmax - h) / 2 + OffsetY,
                     w,
                     h);
}

void n_hud_draw(void)
{
    drawLife(0x63ea82, 0, 4, 4);
    drawLife(0xcdea7d, F_DEG_067_INT, 0, 0);
}
