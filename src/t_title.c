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

#include "t_title.h"
#include "main.h"

static FFont* g_font;

void t_title(void)
{
    F_STATE_INIT
    {
        g_font = f_font_newFromPng("assets/gfx/font_6x8.png", 0, 0, 0, 0);
    }

    F_STATE_TICK
    {
        if(f_button_pressGetOnce(u_input_get(U_BUTTON_ACTION))) {
            f_state_push(t_game_play);
        }
    }

    F_STATE_DRAW
    {
        f_color_blendSet(F_COLOR_BLEND_SOLID);
        f_color_colorSetHex(0xa2a0ab);

        f_draw_fill();

        f_align_push();
        f_align_set(F_ALIGN_X_CENTER, F_ALIGN_Y_CENTER);

        f_font_fontSet(g_font);
        f_color_fillBlitSet(true);
        f_color_colorSetHex(0x4e3d40);

        FVecInt screen = f_screen_sizeGet();
        f_font_coordsSet(screen.x / 2, screen.y / 2);

        f_font_print(
            "Press Z key to start\n"
            "Use arrow keys to move\n\n"
            "Say hi to some, avoid others\n"
            "Keep yourself alive\n\n"
            "www.alxm.org 2020 - Ludum Dare 46"
            );

        f_align_pop();
    }

    F_STATE_FREE
    {
        f_font_fontSet(NULL);
        f_font_free(g_font);
    }
}
