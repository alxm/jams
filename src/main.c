/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_cargo.h"
#include "component_damage.h"
#include "component_health.h"
#include "component_input.h"
#include "component_interact.h"
#include "component_map.h"
#include "component_mood.h"
#include "component_position.h"
#include "component_sprite.h"

#include "system_input.h"
#include "system_interact.h"
#include "system_hud.h"
#include "system_map.h"
#include "system_sprite.h"

#include "state_game.h"
#include "state_load.h"

A_SETUP
{
    a_settings_set("app.title", "Space Station Plunder");
    a_settings_set("app.version", "0.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("app.output.verbose", "yes");
}

A_MAIN
{
    a_component_declare("cargo", z_comp_cargo_size(), NULL);
    a_component_declare("damage", z_comp_damage_size(), NULL);
    a_component_declare("health", z_comp_health_size(), NULL);
    a_component_declare("hud", 0, NULL);
    a_component_declare("input", z_comp_input_size(), NULL);
    a_component_declare("interact", z_comp_interact_size(), z_comp_interact_free);
    a_component_declare("map", z_comp_map_size(), NULL);
    a_component_declare("mood", z_comp_mood_size(), NULL);
    a_component_declare("position", z_comp_position_size(), NULL);
    a_component_declare("sprite", z_comp_sprite_size(), NULL);

    a_system_declare("drawHud", "hud", z_system_hudDraw, NULL, false);
    a_system_declare("drawMap", "map", z_system_mapDraw, NULL, false);
    a_system_declare("drawSprites", "position sprite", z_system_sprite, NULL, false);
    a_system_declare("getInputs", "input", z_system_input, NULL, false);
    a_system_declare("runInteractions", "interact", z_system_interact, NULL, false);

    a_state_new("load", load);
    a_state_new("playGame", playGame);
    a_state_new("nextScreen", nextScreen);

    a_state_push("load");
}
