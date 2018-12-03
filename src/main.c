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

#include "util_state.h"

A_SETUP
{
    a_settings_stringSet(A_SETTING_APP_TITLE, "Cave Shrine");
    a_settings_stringSet(A_SETTING_APP_VERSION, "0.1.0");
    a_settings_stringSet(A_SETTING_APP_AUTHOR, "alxm");
    a_settings_colorSet(A_SETTING_COLOR_SCREEN_BORDER, 0x111111);
}

A_MAIN
{
    u_state_push(U_STATE_ID_LOAD);
}
