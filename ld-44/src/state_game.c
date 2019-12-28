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

#include "obj_game.h"

A_STATE(s_game)
{
    A_STATE_INIT
    {
        n_game_new();
    }

    A_STATE_TICK
    {
        n_game_tick();
    }

    A_STATE_DRAW
    {
        n_game_draw();
    }

    A_STATE_FREE
    {
        n_game_free();
    }
}
