/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_colors.h"
#include "util_controls.h"
#include "util_frames.h"
#include "util_terrain.h"

A_STATE(load)
{
    A_STATE_INIT
    {
        z_util_colors_load();
        z_util_controls_load();
        z_util_frames_load();
        z_util_terrain_load();
    }

    A_STATE_TICK
    {
        a_state_push("game");
    }

    A_STATE_FREE
    {
        z_util_frames_unload();
        z_util_terrain_unload();
    }
}
