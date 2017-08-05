/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_sprites.h"
#include "util_tiles.h"

A_STATE(load)
{
    A_STATE_INIT
    {
        z_util_sprites_load();
        z_util_tiles_load();

        a_state_push("game");
    }

    A_STATE_FREE
    {
        z_util_sprites_free();
        z_util_tiles_free();
    }
}
