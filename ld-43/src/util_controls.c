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

#include "util_controls.h"

static AButton* g_buttons[U_BUTTON_NUM];

void u_controls_load(void)
{
    static const struct {
        int key;
        int button;
    } mappings[U_BUTTON_NUM] = {
        [U_BUTTON_UP] = {A_KEY_UP, A_BUTTON_UP},
        [U_BUTTON_DOWN] = {A_KEY_DOWN, A_BUTTON_DOWN},
        [U_BUTTON_LEFT] = {A_KEY_LEFT, A_BUTTON_LEFT},
        [U_BUTTON_RIGHT] = {A_KEY_RIGHT, A_BUTTON_RIGHT},
        [U_BUTTON_ACTION] = {A_KEY_SPACE, A_BUTTON_A},
    };

    for(UButtonId b = 0; b < U_BUTTON_NUM; b++) {
        g_buttons[b] = a_button_new();

        a_button_bind(g_buttons[b], mappings[b].key);
        a_button_bind(g_buttons[b], mappings[b].button);
    }
}

void u_controls_unload(void)
{
    for(UButtonId b = 0; b < U_BUTTON_NUM; b++) {
        a_button_free(g_buttons[b]);
    }
}

AButton* u_controls_get(UButtonId Id)
{
    return g_buttons[Id];
}
