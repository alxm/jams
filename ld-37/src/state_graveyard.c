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
#include "missions.h"
#include "component_map.h"
#include "component_mapevent.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_volume.h"

static int g_counter;
static int g_treasureIndex;
#define Z_MAX_TO_MARK 4
#define Z_MAX_ATTEMPTS 2

static void z_grave_handlerPlain(AEntity* Door, ZWorld* World)
{
    if(z_missions.isStuck || !z_missions.canEnterGraveyard) {
        z_world_message(World, "Nothing interesting here.", "", 30);
        return;
    }

    z_missions.numAttempts++;

    if(z_missions.numAttempts > Z_MAX_ATTEMPTS) {
        z_world_message(World,
                        "You snooped around too much and was caught by a zombie.",
                        "You live in the graveyard now. Press ESC to quit game.",
                        0);
        z_missions.isStuck = true;
    } else {
        z_world_message(World, "Nothing interesting here.", "", 30);
    }
}

static void z_grave_handlerMarked(AEntity* Door, ZWorld* World)
{
    if(z_missions.isStuck || !z_missions.canEnterGraveyard) {
        z_world_message(World, "Nothing interesting here.", "", 30);
        return;
    }

    z_missions.numAttempts++;

    if(z_missions.numAttempts > Z_MAX_ATTEMPTS) {
        z_world_message(World,
                        "You snooped around too much and was caught by a zombie.",
                        "You live in the graveyard now. Press ESC to quit game.",
                        0);
        z_missions.isStuck = true;
    } else {
        z_world_message(World,
                        "Nothing interesting here.",
                        "Always worth a look though!",
                        30);
    }
}

static void z_grave_handlerTreasure(AEntity* Door, ZWorld* World)
{
    if(z_missions.isStuck || !z_missions.canEnterGraveyard) {
        z_world_message(World, "Nothing interesting here.", "", 30);
        return;
    }

    z_missions.numAttempts++;

    z_world_message(World,
                    "Found some treasure.",
                    "Go back to Room 5 for more missions.",
                    30);

    z_missions.canPhoneRing = true;
    z_missions.canEnterGraveyard = false;
}

static void z_grave_make(int X, int Y, AColMap* Colmap, bool North, bool West)
{
    AEntity* gate = a_entity_new();

    bool marked = false;
    bool hasTreasure = false;
    bool matches = North == z_missions.north && West == z_missions.west;

    if(matches) {
        if(g_counter++ == g_treasureIndex) {
            hasTreasure = true;
            marked = true;
        } else if(a_random_int(2) == 0) {
            marked = true;
        }
    }

    if(hasTreasure) {
        z_comp_mapevent_init(a_entity_addComponent(gate, "mapevent"), z_grave_handlerTreasure);
    } else if(marked) {
        z_comp_mapevent_init(a_entity_addComponent(gate, "mapevent"), z_grave_handlerMarked);
    } else {
        z_comp_mapevent_init(a_entity_addComponent(gate, "mapevent"), z_grave_handlerPlain);
    }

    z_comp_position_init(a_entity_addComponent(gate, "position"), X, Y);

    if(marked) {
        z_comp_sprite_init(a_entity_addComponent(gate, "sprite"), "markedGrave");
    } else {
        z_comp_sprite_init(a_entity_addComponent(gate, "sprite"), "grave");
    }

    z_comp_volume_init(a_entity_addComponent(gate, "volume"), Colmap, X, Y, 16, 24);
}

static void z_gate_handler(AEntity* Door, ZWorld* World)
{
    if(z_missions.isStuck) {
        z_world_message(World,
                        "Sorry, you can't leave. Press ESC to quit game.",
                        "Thanks for playing! Visit www.alxm.org for updates :-)",
                        0);
    } else {
        a_state_pop();
    }
}

static void z_gate_make(int X, int Y, AColMap* Colmap)
{
    AEntity* gate = a_entity_new();

    z_comp_mapevent_init(a_entity_addComponent(gate, "mapevent"), z_gate_handler);
    z_comp_position_init(a_entity_addComponent(gate, "position"), X, Y);
    z_comp_sprite_init(a_entity_addComponent(gate, "sprite"), "gate");
    z_comp_volume_init(a_entity_addComponent(gate, "volume"), Colmap, X, Y, 24, 8);
}

A_STATE(graveyard)
{
    static ZWorld* world;

    A_STATE_INIT
    {
        #define MAP_WIDTH 16
        #define MAP_HEIGHT 10

        ZTileType mapData[MAP_HEIGHT][MAP_WIDTH] = {
            {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,},
            {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,},
            {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,},
            {4,3,3,3,3,3,4,0,0,4,3,3,3,3,3,4,},
            {7,7,7,7,7,7,7,0,0,7,7,7,7,7,7,7,},
            {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,},
        };

        world = z_world_new((ZTileType*)mapData, MAP_WIDTH, MAP_HEIGHT);
        AColMap* colmap = z_world_getColmap(world);

        int y;
        g_counter = 0;
        g_treasureIndex = a_random_int(6);

        // NORTH-WEST
        y = 3 * Z_TILE_DIM - Z_TILE_DIM / 4;
        z_grave_make(1 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, true);
        z_grave_make(2 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, true);
        z_grave_make(3 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, true);
        z_grave_make(4 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, true);
        z_grave_make(5 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, true);
        z_grave_make(6 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, true);

        // SOUTH-WEST
        y = 5 * Z_TILE_DIM + Z_TILE_DIM / 4;
        z_grave_make(1 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, true);
        z_grave_make(2 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, true);
        z_grave_make(3 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, true);
        z_grave_make(4 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, true);
        z_grave_make(5 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, true);
        z_grave_make(6 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, true);

        // NORTH-EAST
        y = 3 * Z_TILE_DIM - Z_TILE_DIM / 4;
        z_grave_make( 9 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, false);
        z_grave_make(10 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, false);
        z_grave_make(11 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, false);
        z_grave_make(12 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, false);
        z_grave_make(13 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, false);
        z_grave_make(14 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, true, false);

        // SOUTH-EAST
        y = 5 * Z_TILE_DIM + Z_TILE_DIM / 4;
        z_grave_make( 9 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, false);
        z_grave_make(10 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, false);
        z_grave_make(11 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, false);
        z_grave_make(12 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, false);
        z_grave_make(13 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, false);
        z_grave_make(14 * Z_TILE_DIM + Z_TILE_DIM / 2, y, colmap, false, false);

        z_gate_make(8 * Z_TILE_DIM, 8 * Z_TILE_DIM, colmap);

        z_world_finalize(world, 128, 104);
    }

    A_STATE_BODY
    {
        A_STATE_LOOP;
    }

    A_STATE_FREE
    {
        z_world_free(world);
    }
}
