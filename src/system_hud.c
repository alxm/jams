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

#include "component_cargo.h"
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
    a_sprite_blit(face, x, y + 2);
    x += a_sprite_width(face) + 4;

    unsigned universeX, universeY;
    z_game_getUniverseCoords(&universeX, &universeY);

    a_font_setFace(A_FONT_FACE_WHITE);
    a_font_setCoords(x, y);
    a_font_textf("@ %u, %u", universeX, universeY);
    a_font_newLine();
    y = a_font_getY() + 2;

    ZCompHealth* health = a_entity_requireComponent(Entity, "health");
    int points, max;
    z_comp_health_getStats(health, &points, &max);
    z_graphics_drawBar(0xd4f65e, 0xef1d07, points, max, x, y, 50, 8);

    ZCompCargo* cargo = a_entity_requireComponent(Entity, "cargo");
    a_font_setCoords(4, y + 8 + 4);

    for(ZCargoType t = 0; t < Z_CARGO_TYPE_NUM; t++) {
        a_font_textf("%s: %d",
                     z_comp_cargo_getName(t, true),
                     z_comp_cargo_getNum(cargo, t));
        a_font_newLine();
    }

    x = 80;
    y = Z_MAP_PIXEL_H + 4;

    a_font_setCoords(x, y);

    int alphaInc = 32;
    int numLines = (int)a_list_size(z_game_getLogLines());
    int alpha = A_PIXEL_ALPHA_MAX - (numLines + 1) * alphaInc;

    a_pixel_push();
    a_pixel_setBlend(A_PIXEL_BLEND_RGBA);

    A_LIST_ITERATE(z_game_getLogLines(), char*, line) {
        if(A_LIST_IS_LAST()) {
            a_pixel_setBlend(A_PIXEL_BLEND_PLAIN);
        } else {
            a_pixel_setAlpha(alpha);
            alpha += alphaInc;
        }

        a_font_setFace(z_fonts.lightBlue);
        a_font_text("> ");
        a_font_setFace(A_FONT_FACE_WHITE);
        a_font_text(line);
        a_font_newLine();
    }

    a_pixel_pop();
}
