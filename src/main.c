/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "state_game.h"
#include "state_load.h"

#include "component_map.h"

#include "system_map.h"

A_SETUP
{
    a_settings_set("app.title", "Pestering Peddler");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("video.width", "400");
    a_settings_set("video.height", "240");
}

A_MAIN
{
    a_component_declare("map", z_comp_map_size(), z_comp_map_free);

    a_system_declare("mapDraw", "map", z_system_mapDraw, NULL, false);
    a_system_declare("mapTick", "map", z_system_mapTick, NULL, false);

    a_state_new("game", game);
    a_state_new("load", load);

    a_state_push("load");
}
