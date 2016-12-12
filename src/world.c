/*
    Copyright 2016 Alex Margarit

    This file is part of Motel 37.

    Motel 37 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Motel 37 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Motel 37.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "tiles.h"
#include "world.h"
#include "component_input.h"
#include "component_map.h"
#include "component_mapevent.h"
#include "component_phone.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_text.h"
#include "component_velocity.h"
#include "component_volume.h"
#include "system_input.h"
#include "system_map.h"
#include "system_messagebox.h"
#include "system_move.h"
#include "system_phone.h"
#include "system_sprite.h"
#include "entity_camera.h"
#include "entity_map.h"
#include "entity_message.h"
#include "entity_player.h"

struct ZWorld {
    AColMap* colmap;
    AEntity* map;
    AEntity* player;
    AEntity* camera;
    AEntity* messageBox;
};

ZWorld* z_world_new(ZTileType* MapData, int MapWidth, int MapHeight)
{
    ZWorld* world = a_mem_malloc(sizeof(ZWorld));

    a_component_declare("input", z_comp_input_size(), NULL);
    a_component_declare("map", z_comp_map_size(), z_comp_map_free);
    a_component_declare("mapevent", z_comp_mapevent_size(), NULL);
    a_component_declare("phone", z_comp_phone_size(), NULL);
    a_component_declare("position", z_comp_position_size(), NULL);
    a_component_declare("sprite", z_comp_sprite_size(), z_comp_sprite_free);
    a_component_declare("text", z_comp_text_size(), NULL);
    a_component_declare("velocity", z_comp_velocity_size(), NULL);
    a_component_declare("volume", z_comp_volume_size(), z_comp_volume_free);

    a_system_declare("input", "input", z_system_input);
    a_system_declare("mapDraw", "map", z_system_mapDraw);
    a_system_declare("move", "position velocity", z_system_move);
    a_system_declare("sprite", "position sprite", z_system_sprite);
    a_system_declare("messageBoxTick", "text", z_system_messageBoxTick);
    a_system_declare("messageBoxDraw", "text", z_system_messageBoxDraw);
    a_system_declare("phone", "phone sprite", z_system_phone);

    a_system_tick("input move messageBoxTick phone");
    a_system_draw("mapDraw sprite messageBoxDraw");

    a_system_setContext(world);

    world->map = z_entity_map_new(MapData, MapWidth, MapHeight);

    int mapW, mapH;
    z_comp_map_getDim(a_entity_getComponent(world->map, "map"), &mapW, &mapH);
    world->colmap = a_colmap_new(mapW * Z_TILE_DIM,
                                 mapH * Z_TILE_DIM,
                                 64);

    world->messageBox = z_entity_message_new();

    return world;
}

void z_world_finalize(ZWorld* World, int PlayerX, int PlayerY)
{
    World->player = z_entity_player_new(PlayerX, PlayerY, World->colmap);
    World->camera = z_entity_camera_new(World->player);
}

void z_world_free(ZWorld* World)
{
    a_entity_free(World->camera);
    a_entity_free(World->player);
    a_colmap_free(World->colmap);
    a_entity_free(World->map);
    free(World);
}

AColMap* z_world_getColmap(const ZWorld* World)
{
    return World->colmap;
}

AEntity* z_world_getMap(const ZWorld* World)
{
    return World->map;
}

AEntity* z_world_getPlayer(const ZWorld* World)
{
    return World->player;
}

AEntity* z_world_getCamera(const ZWorld* World)
{
    return World->camera;
}

void z_world_message(const ZWorld* World, const char* String1, const char* String2, unsigned int Ticks)
{
    ZCompText* text = a_entity_getComponent(World->messageBox, "text");

    z_comp_text_setStrings(text, String1, String2);
    z_comp_text_setTtl(text, Ticks);
}

void z_world_clearMessage(const ZWorld* World)
{
    z_world_message(World, "", "", 0);
}
