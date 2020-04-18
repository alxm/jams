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

#include "t_game.h"
#include "main.h"

void t_game(void)
{
    F_STATE_INIT
    {
        n_cam_new();
        n_map_new();
        o_orb_new();
    }

    F_STATE_TICK
    {
        n_map_tick();
        o_orb_tick();
        n_cam_tick();
    }

    F_STATE_DRAW
    {
        n_map_draw();
        o_orb_draw();
    }

    F_STATE_FREE
    {
        o_orb_free();
        n_map_free();
        n_cam_free();
    }
}
