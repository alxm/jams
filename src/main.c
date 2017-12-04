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

#include "state_game.h"
#include "state_load.h"

#include "util_level.h"
#include "util_terrain.h"

#include "component_cursor.h"
#include "component_mapgfx.h"
#include "component_mapterrain.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_volume.h"

#include "system_cursor.h"
#include "system_map.h"
#include "system_sprite.h"

A_SETUP
{
    a_settings_set("app.title", "Mine Op 40");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("app.output.verbose", "yes");
    a_settings_set("input.hideCursor", "yes");
    a_settings_set("video.borderColor", "0x3e1d5f");
}

A_MAIN
{
    a_component_declare("cursor", z_comp_cursor_size(), z_comp_cursor_free);
    a_component_declare("mapGfx", z_comp_mapgfx_size(), z_comp_mapgfx_free);
    a_component_declare("mapTerrain", z_comp_mapterrain_size(), z_comp_mapterrain_free);
    a_component_declare("position", z_comp_position_size(), NULL);
    a_component_declare("sprite", z_comp_sprite_size(), z_comp_sprite_free);
    a_component_declare("tagBuilding", 0, NULL);
    a_component_declare("tagWorker", 0, NULL);
    a_component_declare("volume", z_comp_volume_size(), z_comp_volume_free);

    a_system_declare("tickCursor", "cursor", z_system_cursorTick, NULL, false);
    a_system_declare("tickMapFrame", "mapGfx", z_system_mapFrame, NULL, false);
    a_system_declare("tickSpriteFrame", "sprite", z_system_spriteTickFrame, NULL, false);

    a_system_declare("drawCursorUnderside", "cursor", z_system_cursorDrawUnderside, NULL, false);
    a_system_declare("drawMapTiles", "mapTerrain", z_system_mapDrawTiles, NULL, false);
    a_system_declare("drawSprite", "position sprite", z_system_spriteDraw, z_system_sprite_sort, false);

    a_state_new("game",
                game,
                "tickCursor tickMapFrame tickSpriteFrame",
                "drawMapTiles drawCursorUnderside drawSprite");

    a_state_new("load", load, "", "");

    a_state_push("load");
}
