/*
    Copyright 2016 Alex Margarit

    This file is part of Motel 37.

    Motel 37 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Motel 37 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Motel 37.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "controls.h"
#include "graphics.h"
#include "tiles.h"

A_STATE(load)
{
    A_STATE_INIT
    {
        z_controls_load();
        z_graphics_load();
        z_tiles_load();

        a_state_push("parkingLot");
    }

    A_STATE_BODY
    {
        a_state_pop();
    }

    A_STATE_FREE
    {
        z_graphics_free();
    }
}
