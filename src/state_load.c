/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    38th Century - A simple world simulation game made for Ludum Dare 38 72h

    38th Century is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    38th Century is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 38th Century.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_controls.h"
#include "util_data.h"
#include "util_graphics.h"
#include "util_log.h"
#include "util_period.h"

#include "state_game.h"

A_STATE(load)
{
    A_STATE_INIT
    {
        z_controls_load();
        z_data_load();
        z_graphics_load();
        z_period_load();
    }

    A_STATE_BODY
    {
        a_state_push("title");
    }

    A_STATE_FREE
    {
        z_graphics_unload();
    }
}

A_STATE(title)
{
    A_STATE_BODY
    {
        uint32_t hiscore = 0;
        AFile* f = a_file_open(Z_HISCORE_FILE, "rb");

        if(f) {
            if(a_file_checkPrefix(f, Z_HISCORE_KEY)) {
                a_file_read(f, &hiscore, sizeof(hiscore));
            }

            a_file_close(f);
        }

        A_STATE_LOOP {
            if(a_button_getOnce(z_controls.start)) {
                a_state_push("play");
            }

            A_STATE_LOOP_DRAW {
                a_pixel_push();
                a_font_push();

                a_pixel_setPixel(z_colors.darkGray);
                a_draw_fill();

                a_font_setAlign(A_FONT_ALIGN_LEFT);
                a_font_setCoords(40,
                                 a_screen_height() / 2
                                    - a_font_getLineHeight() * 9);

                {
                    a_font_setFace(z_fonts.mediumGray);
                    a_font_text(">");
                    a_font_setFace(z_fonts.cyan);
                    a_font_text("38th Century");
                    a_font_newLine();
                }
                {
                    a_font_setFace(z_fonts.mediumGray);
                    a_font_text(">");
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text("Ludum Dare 38 48h 72h");
                    a_pixel_setPixel(z_colors.magenta);

                    a_pixel_push();
                    a_pixel_setBlend(A_PIXEL_BLEND_RGB50);
                    a_draw_line(a_font_getX() - 40,
                                a_font_getY() + a_font_getLineHeight() - 2,
                                a_font_getX() - 20,
                                a_font_getY() - 2);
                    a_pixel_pop();
                    a_font_newLine();
                }
                a_font_newLine();
                {
                    a_font_setFace(z_fonts.mediumGray);
                    a_font_text(">");
                    a_font_setFace(z_fonts.cyan);
                    a_font_textf("HISCORE: %u", hiscore);
                    a_font_newLine();
                }

                a_pixel_setPixel(z_colors.cyan);
                a_pixel_setPixel(z_colors.mediumGray);
                a_font_newLine();
                a_draw_hline(40,
                             a_screen_width() - 40,
                             a_font_getY() + a_font_getLineHeight() / 2);

                a_font_newLine();
                a_font_newLine();
                {
                    a_font_setFace(z_fonts.mediumGray);
                    a_font_text(">");
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text("Use ");
                    a_font_setFace(z_fonts.magenta);
                    a_font_text("arrows");
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text(" to browse menus");
                    a_font_newLine();
                }
                {
                    a_font_setFace(z_fonts.mediumGray);
                    a_font_text(">");
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text("Press ");
                    a_font_setFace(z_fonts.magenta);
                    a_font_text(a_button_name(z_controls.primary));
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text(" to select an option");
                    a_font_newLine();
                }
                a_font_newLine();
                {
                    a_font_setFace(z_fonts.mediumGray);
                    a_font_text(">");
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text("Press ");
                    a_font_setFace(z_fonts.magenta);
                    a_font_text("[Esc]");
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text(" to quit");
                    a_font_newLine();
                }
                {
                    a_font_setFace(z_fonts.mediumGray);
                    a_font_text(">");
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text("Press ");
                    a_font_setFace(z_fonts.magenta);
                    a_font_text("[F4]");
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text(" to toggle fullscreen");
                    a_font_newLine();
                }
                a_font_newLine();
                {
                    a_font_setFace(z_fonts.mediumGray);
                    a_font_text(">");
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text("Press ");
                    a_font_setFace(z_fonts.magenta);
                    a_font_text(a_button_name(z_controls.start));
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text(" to start a new game");
                    a_font_newLine();
                }

                a_font_newLine();
                a_draw_hline(40,
                             a_screen_width() - 40,
                             a_font_getY() + a_font_getLineHeight() / 2);
                a_font_newLine();
                a_font_newLine();

                {
                    a_font_setFace(z_fonts.mediumGray);
                    a_font_text(">");
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text("alxm 2017");
                    a_font_newLine();
                }
                {
                    a_font_setFace(z_fonts.mediumGray);
                    a_font_text(">");
                    a_font_setFace(z_fonts.lightGray);
                    a_font_text("http://www.alxm.org");
                    a_font_newLine();
                }

                a_pixel_pop();
                a_font_pop();

                //z_graphics_drawHelp(Z_HELP_TITLESCREEN);
            }
        }
    }
}
