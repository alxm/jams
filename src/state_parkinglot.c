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

static void z_gravestones_make(void)
{
    int x, y;

    x = 16 * Z_TILE_DIM + Z_TILE_DIM / 2;
    y = 1 * Z_TILE_DIM;

    for(int i = 4; i--; x += 12) {
        AEntity* stone = a_entity_new();
        z_comp_sprite_init(a_entity_addComponent(stone, "sprite"), "stone");
        z_comp_position_init(a_entity_addComponent(stone, "position"), x, y);
    }

    x += 2 * Z_TILE_DIM;

    for(int i = 4; i--; x += 12) {
        AEntity* stone = a_entity_new();
        z_comp_sprite_init(a_entity_addComponent(stone, "sprite"), "stone");
        z_comp_position_init(a_entity_addComponent(stone, "position"), x, y);
    }

    x = 17 * Z_TILE_DIM - Z_TILE_DIM / 4;
    y += Z_TILE_DIM / 2;

    for(int i = 3; i--; x += 12) {
        AEntity* stone = a_entity_new();
        z_comp_sprite_init(a_entity_addComponent(stone, "sprite"), "stone");
        z_comp_position_init(a_entity_addComponent(stone, "position"), x, y);
    }

    x += 3 * Z_TILE_DIM - Z_TILE_DIM / 4;

    for(int i = 4; i--; x += 12) {
        AEntity* stone = a_entity_new();
        z_comp_sprite_init(a_entity_addComponent(stone, "sprite"), "stone");
        z_comp_position_init(a_entity_addComponent(stone, "position"), x, y);
    }
}

static void z_motel_doorHandler1(AEntity* Door, ZWorld* World)
{
    if(z_missions.hasRoom) {
        z_world_message(World,
                        "Breaking and entering is a felony, you know.",
                        "You're staying in Room 5, not 5.",
                        30);
    } else {
        z_world_message(World,
                        "Go to the lobby to get a room.",
                        "",
                        30);
    }
}

static void z_motel_doorHandler2(AEntity* Door, ZWorld* World)
{
    if(z_missions.hasRoom) {
        z_world_message(World,
                        "Breaking and entering is a felony, you know.",
                        "You're staying in Room 5, not 2.",
                        30);
    } else {
        z_world_message(World,
                        "Go to the lobby to get a room.",
                        "",
                        30);
    }
}

static void z_motel_doorHandler3(AEntity* Door, ZWorld* World)
{
    if(z_missions.hasRoom) {
        z_world_message(World,
                        "Breaking and entering is a felony, you know.",
                        "You're staying in Room 5, not 3.",
                        30);
    } else {
        z_world_message(World,
                        "Go to the lobby to get a room.",
                        "",
                        30);
    }
}

static void z_motel_doorHandler4(AEntity* Door, ZWorld* World)
{
    if(z_missions.hasRoom) {
        z_world_message(World,
                       "Breaking and entering is a felony, you know.",
                        "You're staying in Room 5, not 4.",
                        30);
    } else {
        z_world_message(World,
                        "Go to the lobby to get a room.",
                        "",
                        30);
    }
}

static void z_motel_doorHandler5(AEntity* Door, ZWorld* World)
{
    if(z_missions.hasRoom) {
        if(z_missions.canEnterDoor1) {
            z_world_clearMessage(World);
            a_state_push("motelRoom1");
        } else {
            z_world_message(World,
                            "You seem to be locked out of your room...",
                            "Did you lose your key?",
                            30);
        }
    } else {
        z_world_message(World,
                        "Go to the lobby to get a room.",
                        "",
                        30);
    }
}

static void z_motel_doorHandler6(AEntity* Door, ZWorld* World)
{
    if(z_missions.hasRoom) {
        z_world_message(World,
                        "Breaking and entering is a felony, you know.",
                        "You're staying in Room 5, not 6.",
                        30);
    } else {
        z_world_message(World,
                        "Go to the lobby to get a room.",
                        "",
                        30);
    }
}

static void z_motel_lobbyDoorHandler(AEntity* Door, ZWorld* World)
{
    if(z_missions.hasRoom) {
        z_world_message(World,
                        "\"Sorry pal, you can only get one room as per our",
                        "Ludum Dare 37 policy. You're over in Room 5.\"",
                        90);
    } else {
        z_missions.hasRoom = true;
        z_missions.canEnterDoor1 = true;

        z_world_message(World,
                        "\"Welcome to Motel 37! Here's your key, you're staying in ",
                        "Room 5.\"",
                        0);
    }
}

static void z_motel_make(int X, int Y, AColMap* Colmap)
{
    AEntity* building = a_entity_new();
    z_comp_sprite_init(a_entity_addComponent(building, "sprite"), "motel");
    z_comp_position_init(a_entity_addComponent(building, "position"), X, Y);

    AEntity* door;
    int doorX = X - 120 + 3 + 10 / 2;
    int doorY = Y - 32 + 54 + 10 / 2;

    door = a_entity_new();
    z_comp_mapevent_init(a_entity_addComponent(door, "mapevent"), z_motel_doorHandler1);
    z_comp_position_init(a_entity_addComponent(door, "position"), doorX, doorY);
    z_comp_volume_init(a_entity_addComponent(door, "volume"), Colmap, doorX, doorY, 10, 2);

    doorX += 48;
    door = a_entity_new();
    z_comp_mapevent_init(a_entity_addComponent(door, "mapevent"), z_motel_doorHandler2);
    z_comp_position_init(a_entity_addComponent(door, "position"), doorX, doorY);
    z_comp_volume_init(a_entity_addComponent(door, "volume"), Colmap, doorX, doorY, 10, 2);

    doorX += 16;
    door = a_entity_new();
    z_comp_mapevent_init(a_entity_addComponent(door, "mapevent"), z_motel_doorHandler3);
    z_comp_position_init(a_entity_addComponent(door, "position"), doorX, doorY);
    z_comp_volume_init(a_entity_addComponent(door, "volume"), Colmap, doorX, doorY, 10, 2);

    doorX += 48;
    door = a_entity_new();
    z_comp_mapevent_init(a_entity_addComponent(door, "mapevent"), z_motel_doorHandler4);
    z_comp_position_init(a_entity_addComponent(door, "position"), doorX, doorY);
    z_comp_volume_init(a_entity_addComponent(door, "volume"), Colmap, doorX, doorY, 10, 2);

    doorX += 16;
    door = a_entity_new();
    z_comp_mapevent_init(a_entity_addComponent(door, "mapevent"), z_motel_doorHandler5);
    z_comp_position_init(a_entity_addComponent(door, "position"), doorX, doorY);
    z_comp_volume_init(a_entity_addComponent(door, "volume"), Colmap, doorX, doorY, 10, 2);

    doorX += 48;
    door = a_entity_new();
    z_comp_mapevent_init(a_entity_addComponent(door, "mapevent"), z_motel_doorHandler6);
    z_comp_position_init(a_entity_addComponent(door, "position"), doorX, doorY);
    z_comp_volume_init(a_entity_addComponent(door, "volume"), Colmap, doorX, doorY, 10, 2);

    doorX += 32;
    door = a_entity_new();
    z_comp_mapevent_init(a_entity_addComponent(door, "mapevent"), z_motel_lobbyDoorHandler);
    z_comp_position_init(a_entity_addComponent(door, "position"), doorX, doorY);
    z_comp_volume_init(a_entity_addComponent(door, "volume"), Colmap, doorX, doorY, 10, 2);
}

static void z_gate_handler(AEntity* Door, ZWorld* World)
{
    if(z_missions.canEnterGraveyard) {
        z_world_clearMessage(World);
        a_state_push("graveyard");
    } else {
        if(z_missions.hasRoom) {
            z_missions.canPhoneRing = true;
            z_world_message(World,
                            "Why would you want to go to the graveyard?",
                            "Room 5 is much more comfortable!",
                            60);
        } else {
            z_world_message(World,
                            "Why would you want to go here?",
                            "Go to the motel lobby to get a room.",
                            60);
        }
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

void z_sign_make(int X, int Y, AColMap* Colmap)
{
    AEntity* sign = a_entity_new();

    z_comp_position_init(a_entity_addComponent(sign, "position"), X, Y);
    z_comp_volume_init(a_entity_addComponent(sign, "volume"), Colmap, X, Y, 40, 50);

    ZCompSprite* sprite = a_entity_addComponent(sign, "sprite");
    z_comp_sprite_init(sprite, "motelSign");
    z_comp_sprite_move(sprite);
}

A_STATE(parkingLot)
{
    static ZWorld* world;

    A_STATE_INIT
    {
        #define MAP_WIDTH 24
        #define MAP_HEIGHT 8

        ZTileType mapData[MAP_HEIGHT][MAP_WIDTH] = {
            {5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,},
            {0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,0,0,0,},
            {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,7,7,7,},
            {0,0,7,7,0,0,7,7,0,0,7,7,0,7,0,7,0,2,2,0,0,2,2,0,},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
            {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,},
        };

        world = z_world_new((ZTileType*)mapData, MAP_WIDTH, MAP_HEIGHT);
        AColMap* colmap = z_world_getColmap(world);

        z_gravestones_make();
        z_motel_make(8 * Z_TILE_DIM + Z_TILE_DIM / 2, 2 * Z_TILE_DIM, colmap);
        z_gate_make(20 * Z_TILE_DIM, 2 * Z_TILE_DIM, colmap);
        z_sign_make(17 * Z_TILE_DIM, 3 * Z_TILE_DIM, colmap);

        z_world_finalize(world, 15 * Z_TILE_DIM, 6 * Z_TILE_DIM);

        z_world_message(world,
                        "After a long journey, you arrived at Motel 37.",
                        "Visit the lobby to get a room.",
                        0);
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
