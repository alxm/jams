/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of City, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "util_input.h"

static AButton* g_buttons[U_BUTTON_NUM];

void u_input_load(void)
{
    for(int b = 0; b < U_BUTTON_NUM; b++) {
        g_buttons[b] = a_button_new();
    }

    a_button_bind(g_buttons[U_BUTTON_UP], A_KEY_UP);
    a_button_bind(g_buttons[U_BUTTON_UP], A_BUTTON_UP);

    a_button_bind(g_buttons[U_BUTTON_DOWN], A_KEY_DOWN);
    a_button_bind(g_buttons[U_BUTTON_DOWN], A_BUTTON_DOWN);

    a_button_bind(g_buttons[U_BUTTON_LEFT], A_KEY_LEFT);
    a_button_bind(g_buttons[U_BUTTON_LEFT], A_BUTTON_LEFT);

    a_button_bind(g_buttons[U_BUTTON_RIGHT], A_KEY_RIGHT);
    a_button_bind(g_buttons[U_BUTTON_RIGHT], A_BUTTON_RIGHT);

    a_button_bind(g_buttons[U_BUTTON_ACTION], A_KEY_SPACE);
    a_button_bind(g_buttons[U_BUTTON_ACTION], A_BUTTON_A);
}

void u_input_unload(void)
{
    for(int b = 0; b < U_BUTTON_NUM; b++) {
        a_button_free(g_buttons[b]);
    }
}

AButton* u_input_get(UButtonId Id)
{
    return g_buttons[Id];
}
