/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Cave Shrine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cave Shrine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cave Shrine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

A_SETUP
{
    a_settings_stringSet(A_SETTING_APP_TITLE, "Cave Shrine");
    a_settings_stringSet(A_SETTING_APP_VERSION, "0.1.0");
    a_settings_stringSet(A_SETTING_APP_AUTHOR, "alxm");
}

A_STATE(drawBox)
{
    static struct {
        int x, y;
        AButton* up;
        AButton* down;
        AButton* left;
        AButton* right;
    } context;

    A_STATE_INIT
    {
        context.x = a_screen_widthGet() / 2;
        context.y = a_screen_heightGet() / 2;

        context.up = a_button_new();
        a_button_bind(context.up, A_KEY_UP);
        a_button_bind(context.up, A_BUTTON_UP);

        context.down = a_button_new();
        a_button_bind(context.down, A_KEY_DOWN);
        a_button_bind(context.down, A_BUTTON_DOWN);

        context.left = a_button_new();
        a_button_bind(context.left, A_KEY_LEFT);
        a_button_bind(context.left, A_BUTTON_LEFT);

        context.right = a_button_new();
        a_button_bind(context.right, A_KEY_RIGHT);
        a_button_bind(context.right, A_BUTTON_RIGHT);
    }

    A_STATE_TICK
    {
        if(a_button_pressGet(context.up)) {
            context.y--;
        } else if(a_button_pressGet(context.down)) {
            context.y++;
        }

        if(a_button_pressGet(context.left)) {
            context.x--;
        } else if(a_button_pressGet(context.right)) {
            context.x++;
        }
    }

    A_STATE_DRAW
    {
        a_pixel_colorSetHex(0xaaff88);
        a_draw_fill();

        a_pixel_colorSetHex(0xffaa44);
        a_draw_rectangle(context.x - 40, context.y - 40, 80, 80);
    }

    A_STATE_FREE
    {
        a_button_free(context.up);
        a_button_free(context.down);
        a_button_free(context.left);
        a_button_free(context.right);
    }
}

A_MAIN
{
    a_state_push(drawBox, "Draw Box");
}
