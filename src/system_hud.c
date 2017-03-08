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

#include "component_health.h"

#include "state_game.h"

void z_system_hudDraw(AEntity* Entity)
{
    Entity = Entity;

    int hudWidth = a_screen_width() - Z_MAP_PIXEL_W;

    a_pixel_setHex(0);
    a_draw_rectangle(Z_MAP_PIXEL_W, 0, hudWidth, a_screen_height());

    unsigned x, y;
    z_game_getUniverseCoords(&x, &y);

    a_font_setFace(A_FONT_FACE_WHITE);
    a_font_setCoords(Z_MAP_PIXEL_W + 4, 4);
    a_font_textf("@ %u, %u", x, y);
    a_font_newLine();

    ZCompHealth* health = a_entity_requireComponent(z_game_getPlayerEntity(),
                                                    "health");
    int points, max;
    z_comp_health_getStats(health, &points, &max);

    int totalWidth = hudWidth - 4;
    int greenWidth = totalWidth * points / max;
    int redWidth = totalWidth - greenWidth;

    a_pixel_setHex(0x00bb00);
    a_draw_rectangle(Z_MAP_PIXEL_W + 2,
                     a_font_getY() + 2,
                     greenWidth,
                     8);

    a_pixel_setHex(0xbb0000);
    a_draw_rectangle(Z_MAP_PIXEL_W + 2 + greenWidth,
                     a_font_getY() + 2,
                     redWidth,
                     8);
}
