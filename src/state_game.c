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

#include "entity_map.h"
#include "entity_player.h"

A_STATE(game)
{
    A_STATE_INIT
    {
        a_system_tick("mapTick");
        a_system_draw("mapDraw spriteDraw");

        z_entity_map_new("gfx/level00.png");
        z_entity_player_new(2, 5);
    }

    A_STATE_BODY
    {
        A_STATE_LOOP;
    }
}
