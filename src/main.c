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

A_SETUP
{
    a_settings_set("app.title", "Mine Op 40");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
}

A_STATE(drawBox)
{
    static struct {
        int x, y;
        AInputButton* up;
        AInputButton* down;
        AInputButton* left;
        AInputButton* right;
    } context;

    A_STATE_INIT
    {
        context.x = a_screen_getWidth() / 2;
        context.y = a_screen_getHeight() / 2;

        context.up = a_button_new("key.up");
        context.down = a_button_new("key.down");
        context.left = a_button_new("key.left");
        context.right = a_button_new("key.right");
    }

    A_STATE_TICK
    {
        if(a_button_getPressed(context.up)) {
            context.y--;
        } else if(a_button_getPressed(context.down)) {
            context.y++;
        }

        if(a_button_getPressed(context.left)) {
            context.x--;
        } else if(a_button_getPressed(context.right)) {
            context.x++;
        }
    }

    A_STATE_DRAW
    {
        a_pixel_setHex(0xaaff88);
        a_draw_fill();

        a_pixel_setHex(0xffaa44);
        a_draw_rectangle(context.x - 40, context.y - 40, 80, 80);
    }
}

A_MAIN
{
    a_state_new("drawBox", drawBox, "", "");
    a_state_push("drawBox");
}
