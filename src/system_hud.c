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
#include "component_mood.h"

#include "state_game.h"

void z_system_hudDraw(AEntity* Entity)
{
    int x = 0;
    int y = Z_MAP_PIXEL_H;

    a_pixel_setHex(0x432949);
    a_draw_hline(0, a_screen_width(), y);
    y++;

    a_pixel_setHex(0x151225);
    a_draw_rectangle(x, y, a_screen_width(), a_screen_height() - y);

    x += 4;
    y += 4;

    ZCompMood* mood = a_entity_requireComponent(Entity, "mood");
    ZMoodType moodType = z_comp_mood_getType(mood);

    ASprite* face = z_graphics_getFrame(z_graphics_get("goodbad"),
                                        moodType == Z_MOOD_GOOD ? 0 : 1);
    a_sprite_blit(face, x, y);
    x += a_sprite_width(face) + 4;

    unsigned universeX, universeY;
    z_game_getUniverseCoords(&universeX, &universeY);

    a_font_setFace(A_FONT_FACE_WHITE);
    a_font_setCoords(x, y);
    a_font_textf("@ %u, %u", universeX, universeY);
    a_font_newLine();
    y = a_font_getY() + 2;

    ZCompHealth* health = a_entity_requireComponent(Entity,
                                                    "health");
    int points, max;
    z_comp_health_getStats(health, &points, &max);

    int totalWidth = 24;
    int greenWidth = totalWidth * points / max;
    int redWidth = totalWidth - greenWidth;
    int barHeight = 8;

    a_pixel_setHex(0x00bb00);
    a_draw_rectangle(x, y, greenWidth, barHeight);

    a_pixel_setHex(0xbb0000);
    a_draw_rectangle(x + greenWidth, y, redWidth, barHeight);

    x = 80;
    y = Z_MAP_PIXEL_H + 4;

    a_font_setCoords(x, y);

    A_LIST_ITERATE_BACKWARDS(z_game_getLogLines(), char*, line) {
        a_font_text(line);
        a_font_newLine();

        if(A_LIST_IS_FIRST()) {
            a_font_setFace(A_FONT_FACE_LIGHT_GRAY);
        }
    }
}
