/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Despot 3900 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Despot 3900 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Despot 3900.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <a2x.h>

A_STATE(drawBox);

A_SETUP
{
    a_settings_set("app.title", "Despot 3900");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
}

A_MAIN
{
    a_state_new("drawBox", drawBox);
    a_state_push("drawBox");
}

A_STATE(drawBox)
{
    A_STATE_BODY
    {
        int x = a_screen_getWidth() / 2;
        int y = a_screen_getHeight() / 2;
        int dim = a_screen_getWidth() / 4;

        AInputButton* up = a_button_new("key.up");
        AInputButton* down = a_button_new("key.down");
        AInputButton* left = a_button_new("key.left");
        AInputButton* right = a_button_new("key.right");

        A_STATE_LOOP
        {
            if(a_button_getPressed(up)) {
                y--;
            } else if(a_button_getPressed(down)) {
                y++;
            }

            if(a_button_getPressed(left)) {
                x--;
            } else if(a_button_getPressed(right)) {
                x++;
            }

            A_STATE_LOOP_DRAW
            {
                a_pixel_setHex(0xaaff88);
                a_draw_fill();

                a_pixel_setHex(0xffaa44);
                a_draw_rectangle(x - dim / 2, y - dim / 2, dim, dim);
            }
        }
    }
}
