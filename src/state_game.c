/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of Coffin Digital, a video game.

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

#include "state_game.h"

#include "util_gfx.h"
#include "util_input.h"

A_STATE(s_game)
{
    A_STATE_INIT
    {
        //
    }

    A_STATE_TICK
    {
        //
    }

    A_STATE_DRAW
    {
        a_color_blendSet(A_COLOR_BLEND_PLAIN);
        a_sprite_blit(u_gfx_get(U_GFX_SCREEN), 0, 0);

        a_font_coordsSet(371, 60);
        a_font_printf("SAT, Week 8");

        a_color_blendSet(A_COLOR_BLEND_MOD);
        a_sprite_blit(u_gfx_getNext(U_GFX_NOISE), 0, 0);
    }

    A_STATE_FREE
    {
        //
    }
}
