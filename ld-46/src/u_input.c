/*
    Copyright 2020 Alex Margarit <alex@alxm.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "u_input.h"
#include "main.h"

static FController* g_gamepad;
static FButton* g_buttons[U_BUTTON_NUM];

static void bindButtons(const FController* Controller)
{
    f_button_bindButton(g_buttons[U_BUTTON_UP], Controller, F_BUTTON_UP);
    f_button_bindButton(g_buttons[U_BUTTON_DOWN], Controller, F_BUTTON_DOWN);
    f_button_bindButton(g_buttons[U_BUTTON_LEFT], Controller, F_BUTTON_LEFT);
    f_button_bindButton(g_buttons[U_BUTTON_RIGHT], Controller, F_BUTTON_RIGHT);
    f_button_bindButton(g_buttons[U_BUTTON_ACTION], Controller, F_BUTTON_A);
}

void u_input_init(void)
{
    for(int b = 0; b < U_BUTTON_NUM; b++) {
        g_buttons[b] = f_button_new();
    }

    g_gamepad = f_controller_new(bindButtons);

    f_button_bindKey(g_buttons[U_BUTTON_UP], F_KEY_UP);
    f_button_bindKey(g_buttons[U_BUTTON_DOWN], F_KEY_DOWN);
    f_button_bindKey(g_buttons[U_BUTTON_LEFT], F_KEY_LEFT);
    f_button_bindKey(g_buttons[U_BUTTON_RIGHT], F_KEY_RIGHT);
    f_button_bindKey(g_buttons[U_BUTTON_ACTION], F_KEY_SPACE);
}

void u_input_uninit(void)
{
    for(int b = 0; b < U_BUTTON_NUM; b++) {
        f_button_free(g_buttons[b]);
    }

    f_controller_free(g_gamepad);
}

FButton* u_input_get(UButtonId Id)
{
    return g_buttons[Id];
}
