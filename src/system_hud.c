/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_graphics.h"

#include "state_game.h"

void z_system_hudDraw(AEntity* Entity)
{
    Entity = Entity;

    a_pixel_setHex(0);
    a_draw_rectangle(Z_MAP_PIXEL_W,
                     0,
                     a_screen_width() - Z_MAP_PIXEL_W,
                     a_screen_height());

    unsigned x, y;
    z_game_getUniverseCoords(&x, &y);

    a_font_setFace(A_FONT_FACE_WHITE);
    a_font_setCoords(Z_MAP_PIXEL_W + 4, 4);
    a_font_textf("@ %u, %u", x, y);
}
