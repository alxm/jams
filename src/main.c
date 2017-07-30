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

#include "state_game.h"

#include "util_controls.h"

A_SETUP
{
    a_settings_set("app.title", "Despot 3900");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
}

A_MAIN
{
    z_controls_load();

    a_state_new("game", game);
    a_state_new("actionMenu", actionMenu);

    a_state_push("game");
}
