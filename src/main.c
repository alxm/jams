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

#include "component_accosted.h"
#include "component_ai.h"
#include "component_bag.h"
#include "component_input.h"
#include "component_item.h"
#include "component_log.h"
#include "component_map.h"
#include "component_motion.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_ttl.h"
#include "component_velocity.h"
#include "component_volume.h"

#include "system_ai.h"
#include "system_animate.h"
#include "system_input.h"
#include "system_log.h"
#include "system_map.h"
#include "system_move.h"
#include "system_sprite.h"
#include "system_ttl.h"

A_SETUP
{
    a_settings_set("app.title", "Pestering Peddler");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("video.width", "400");
    a_settings_set("video.height", "240");
    a_settings_set("video.borderColor", "0x111111");
    a_settings_set("video.fps", "30");
}

A_MAIN
{
    a_component_declare("accosted", z_comp_accosted_size(), NULL);
    a_component_declare("ai", z_comp_ai_size(), z_comp_ai_free);
    a_component_declare("bag", z_comp_bag_size(), z_comp_bag_free);
    a_component_declare("input", z_comp_input_size(), z_comp_input_free);
    a_component_declare("item", z_comp_item_size(), z_comp_item_free);
    a_component_declare("log", z_comp_log_size(), z_comp_log_free);
    a_component_declare("map", z_comp_map_size(), z_comp_map_free);
    a_component_declare("motion", z_comp_motion_size(), NULL);
    a_component_declare("position", z_comp_position_size(), NULL);
    a_component_declare("sprite", z_comp_sprite_size(), NULL);
    a_component_declare("ttl", z_comp_ttl_size(), NULL);
    a_component_declare("velocity", z_comp_velocity_size(), NULL);
    a_component_declare("volume", z_comp_volume_size(), z_comp_volume_free);

    a_system_declare("ai", "ai", z_system_ai, NULL, false);
    a_system_declare("animate", "sprite", z_system_animate, NULL, false);
    a_system_declare("input", "input", z_system_input, NULL, false);
    a_system_declare("logDraw", "log", z_system_logDraw, NULL, false);
    a_system_declare("logTick", "log", z_system_logTick, NULL, false);
    a_system_declare("mapDraw", "map", z_system_mapDraw, NULL, false);
    a_system_declare("move", "position velocity", z_system_move, NULL, false);
    a_system_declare("spriteDraw", "position sprite", z_system_sprite, z_system_sprite_sort, false);
    a_system_declare("ttl", "ttl", z_system_ttl, NULL, false);

    a_state_new("game", game);
    a_state_new("load", load);

    a_state_push("load");
}
