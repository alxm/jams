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
#include "component_phone.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_volume.h"

static void z_door_handler(AEntity* Door, ZWorld* World)
{
    a_state_pop();
}

static void z_phone_handler(AEntity* Entity, ZWorld* World)
{
    ZCompPhone* phone = a_entity_getComponent(Entity, "phone");

    if(z_comp_phone_isRinging(phone)) {
        z_missions.canPhoneRing = false;
        z_comp_phone_setRinging(phone, false);

        z_missions_getNewMission();

        z_world_message(World,
                        z_missions_getMissionClues(),
                        "of the cemetery. I'll make it worth your while.\"",
                        0);

        z_missions.canEnterGraveyard = true;
    } else {
        z_world_message(World,
                        "Nobody's calling and there's no room service.",
                        "Perhaps you should take a walk.",
                        90);
    }
}

static void z_light_handler(AEntity* Entity, ZWorld* World)
{
    if(z_missions.stoleLightBulb) {
        z_world_message(World,
                        "You already took the light bulb, and the drawers are",
                        "empty...",
                        60);
    } else {
        z_missions.stoleLightBulb = true;
        z_world_message(World,
                        "You stole the light bulb from the lamp.",
                        "Want to take the copper wire from the walls, too?",
                        90);
    }
}

static void z_bed_handler(AEntity* Entity, ZWorld* World)
{
    z_world_message(World,
                    "Good thing you don't have a blacklight...",
                    "",
                    30);
}

static void z_door_make(AColMap* Colmap, int X, int Y)
{
    AEntity* door = a_entity_new();

    z_comp_mapevent_init(a_entity_addComponent(door, "mapevent"), z_door_handler);
    z_comp_position_init(a_entity_addComponent(door, "position"), X, Y);
    z_comp_volume_init(a_entity_addComponent(door, "volume"), Colmap, X, Y, 8, 8);
}

static void z_bed_make(AColMap* Colmap, int X, int Y)
{
    AEntity* bed = a_entity_new();

    z_comp_mapevent_init(a_entity_addComponent(bed, "mapevent"), z_bed_handler);
    z_comp_position_init(a_entity_addComponent(bed, "position"), X, Y);
    z_comp_sprite_init(a_entity_addComponent(bed, "sprite"), "bed");
    z_comp_volume_init(a_entity_addComponent(bed, "volume"), Colmap, X, Y, 32, 41);
}

static void z_nightstand1_make(AColMap* Colmap, int X, int Y)
{
    AEntity* bed = a_entity_new();

    z_comp_mapevent_init(a_entity_addComponent(bed, "mapevent"), z_phone_handler);
    z_comp_phone_init(a_entity_addComponent(bed, "phone"));
    z_comp_position_init(a_entity_addComponent(bed, "position"), X, Y);
    z_comp_sprite_init(a_entity_addComponent(bed, "sprite"), "nightstand1");
    z_comp_volume_init(a_entity_addComponent(bed, "volume"), Colmap, X, Y, 16, 24);
}

static void z_nightstand2_make(AColMap* Colmap, int X, int Y)
{
    AEntity* bed = a_entity_new();

    z_comp_mapevent_init(a_entity_addComponent(bed, "mapevent"), z_light_handler);
    z_comp_position_init(a_entity_addComponent(bed, "position"), X, Y);
    z_comp_sprite_init(a_entity_addComponent(bed, "sprite"), "nightstand2");
    z_comp_volume_init(a_entity_addComponent(bed, "volume"), Colmap, X, Y, 16, 24);
}

A_STATE(motelRoom1)
{
    static ZWorld* world;

    A_STATE_INIT
    {
        #define MAP_WIDTH 8
        #define MAP_HEIGHT 9

        ZTileType mapData[MAP_HEIGHT][MAP_WIDTH] = {
            {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
            {0x8,0xc,0xc,0xc,0xc,0xc,0xc,0x9,},
            {0x8,0xa,0xa,0xa,0xa,0xa,0xa,0x9,},
            {0x8,0xe,0xe,0xe,0xe,0xe,0xe,0x9,},
            {0x8,0xe,0xe,0xe,0xe,0xe,0xe,0x9,},
            {0x8,0xe,0xe,0xe,0xe,0xe,0xe,0x9,},
            {0x8,0xe,0xe,0xe,0xe,0xe,0xe,0x9,},
            {0x8,0xa,0xd,0xa,0xb,0xb,0xa,0x9,},
            {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},
        };

        world = z_world_new((ZTileType*)mapData, MAP_WIDTH, MAP_HEIGHT);
        AColMap* colmap = z_world_getColmap(world);

        z_nightstand1_make(colmap, 2 * Z_TILE_DIM + Z_TILE_DIM / 2, 3 * Z_TILE_DIM);
        z_bed_make(colmap, 4 * Z_TILE_DIM, 3 * Z_TILE_DIM + Z_TILE_DIM / 2);
        z_nightstand2_make(colmap, 5 * Z_TILE_DIM + Z_TILE_DIM / 2, 3 * Z_TILE_DIM);
        z_door_make(colmap, 2 * Z_TILE_DIM + Z_TILE_DIM / 2, 7 * Z_TILE_DIM + Z_TILE_DIM / 2);

        z_world_finalize(world, 2 * Z_TILE_DIM + Z_TILE_DIM / 2, 6 * Z_TILE_DIM + Z_TILE_DIM / 2);
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
