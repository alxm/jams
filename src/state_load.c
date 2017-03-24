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

#include "util_controls.h"
#include "util_graphics.h"

A_STATE(load)
{
    A_STATE_INIT
    {
        z_controls_load();
        z_graphics_load();
    }

    A_STATE_BODY
    {
        A_STATE_LOOP {
            A_STATE_LOOP_DRAW {
                a_pixel_setHex(0x151225);
                a_draw_fill();

                a_font_push();

                a_font_setFace(z_fonts.lightBlue);
                a_font_setAlign(A_FONT_ALIGN_MIDDLE);
                a_font_setCoords(a_screen_width() / 2,
                                 a_screen_height() / 2
                                    - a_font_getLineHeight() * 5);

                ASprite* sprite;
                const ZGraphic* graphic;

                graphic = z_graphics_get("ship1Up");
                sprite = z_graphics_getFrame(graphic, 0);
                a_sprite_blit(sprite, 44, a_font_getY());

                graphic = z_graphics_get("satellite1");
                sprite = z_graphics_getFrame(graphic, 0);
                a_sprite_blit(sprite,
                              a_screen_width() - 44 - a_sprite_width(sprite),
                              a_font_getY());

                a_font_text("Space Station Plunder - 7DRL 2017");
                a_font_newLine();
                a_font_text("www.alxm.org");
                a_font_newLine();
                a_font_newLine();

                a_pixel_setHex(0x432949);
                a_draw_hline(40,
                             a_screen_width() - 40,
                             a_font_getY() + a_font_getLineHeight() / 2);
                a_font_newLine();
                a_font_newLine();

                a_font_setFace(A_FONT_FACE_WHITE);
                a_font_text("Use arrows to move");
                a_font_newLine();
                a_font_textf("%s changes attitude",
                             a_button_name(z_controls.primary));
                a_font_newLine();
                a_font_textf("%s selects menus, %s goes back",
                             a_button_name(z_controls.primary),
                             a_button_name(z_controls.secondary));
                a_font_newLine();
                a_font_newLine();

                a_font_textf("Press %s to start, %s to exit",
                             a_button_name(z_controls.primary),
                             a_button_name(z_controls.secondary));
                a_font_newLine();
                a_font_text("ESC quits at any time");

                a_font_pop();

                if(a_button_getOnce(z_controls.primary)) {
                    a_state_push("playGame");
                } else if(a_button_getOnce(z_controls.secondary)) {
                    a_state_pop();
                }
            }
        }
    }

    A_STATE_FREE
    {
        z_graphics_unload();
    }
}
