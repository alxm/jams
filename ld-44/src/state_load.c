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

#include "state_load.h"

#include "util_color.h"
#include "util_font.h"
#include "util_gfx.h"
#include "util_input.h"
#include "util_state.h"

void s_load(void)
{
    A_STATE_INIT
    {
        u_color_load();
        u_gfx_load();
        u_font_load();
        u_input_load();

        a_state_push(U_STATE_GAME);
    }

    A_STATE_FREE
    {
        u_input_unload();
        u_font_unload();
        u_gfx_unload();
    }
}
