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

#include "util_graphics.h"

#include "component_ai.h"
#include "component_cargo.h"
#include "component_damage.h"
#include "component_health.h"
#include "component_interact.h"
#include "component_map.h"
#include "component_mood.h"
#include "component_position.h"
#include "component_sprite.h"

#include "entity_macros.h"

#include "state_game.h"

typedef struct ZAiContextShip {
    int annoyedCounter;
} ZAiContextShip;

static void shipAi(AEntity* Entity, ZAiMessageType Type, AEntity* Relevant)
{
    ZCompAi* ai = a_entity_requireComponent(Entity, "ai");
    ZCompMood* mood = a_entity_requireComponent(Entity, "mood");

    ZCompInteract* actorInteract = a_entity_requireComponent(Relevant,
                                                             "interact");
    const char* actorName = z_comp_interact_getName(actorInteract);

    ZAiContextShip* context = z_comp_ai_getContext(ai);
    ZMoodType currentMood = z_comp_mood_getType(mood);

    switch(Type) {
        case Z_AI_MESSAGE_GREETED: {
            if(currentMood == Z_MOOD_GOOD) {
                z_game_log("Hello to you too, %s", actorName);
            } else {
                if(context->annoyedCounter > 0) {
                    z_game_log("Get lost, %s...", actorName);

                    if(--context->annoyedCounter == 0) {
                        z_comp_mood_setType(mood, Z_MOOD_GOOD);
                    }
                } else {
                    z_game_log("* Radio silence *");
                }
            }
        } break;

        case Z_AI_MESSAGE_ATTACKED: {
            context->annoyedCounter++;

            // Retaliate
            z_comp_mood_setType(mood, Z_MOOD_EVIL);

            ZCompPosition* myPos = a_entity_requireComponent(Entity,
                                                             "position");
            int myX, myY;
            z_comp_position_getCoords(myPos, &myX, &myY);

            ZCompPosition* actorPos = a_entity_requireComponent(Relevant,
                                                                "position");
            int actorX, actorY;
            z_comp_position_getCoords(actorPos, &actorX, &actorY);

            if(myX < actorX) {
                z_entity_macro_move(Entity, Z_MOVE_RIGHT);
            } else if(myX > actorX) {
                z_entity_macro_move(Entity, Z_MOVE_LEFT);
            } else if(myY < actorY) {
                z_entity_macro_move(Entity, Z_MOVE_DOWN);
            } else if(myY > actorY) {
                z_entity_macro_move(Entity, Z_MOVE_UP);
            }
        } break;
    }
}

static AEntity* spawn(ZCompMap* Map, const char* Name, const char* Up, const char* Down, const char* Left, const char* Right)
{
    AEntity* e = a_entity_new();
    a_entity_setId(e, Name);

    int x, y;
    ZCompPosition* position = a_entity_addComponent(e, "position");

    do {
        x = 1 + a_random_int(Z_MAP_TILES_W - 2);
        y = 1 + a_random_int(Z_MAP_TILES_H - 2);
    } while(z_comp_map_getTileEntity(Map, x, y) != NULL);

    z_comp_position_init(position, x, y);
    z_comp_map_setTileEntity(Map, x, y, e);

    ZCompSprite* sprite = a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite, Up, Down, Left, Right);

    ZCompInteract* interact = a_entity_addComponent(e, "interact");
    z_comp_interact_init(interact, Name);

    return e;
}

static void addAiShip(AEntity* Entity)
{
    ZCompAi* ai = a_entity_addComponent(Entity, "ai");
    z_comp_ai_init(ai, shipAi, sizeof(ZAiContextShip));

    ZAiContextShip* context = z_comp_ai_getContext(ai);
    context->annoyedCounter = 0;
}

static void addCargo(AEntity* Entity, ZCargoType Type, int Number)
{
    ZCompCargo* cargo = a_entity_getComponent(Entity, "cargo");

    if(cargo == NULL) {
        cargo = a_entity_addComponent(Entity, "cargo");
        z_comp_cargo_init(cargo);
    }

    z_comp_cargo_add(cargo, Type, Number);
}

static void addDamage(AEntity* Entity, int Points)
{
    ZCompDamage* damage = a_entity_addComponent(Entity, "damage");
    z_comp_damage_init(damage, Points);
}

static void addHealth(AEntity* Entity, int Points)
{
    ZCompHealth* health = a_entity_addComponent(Entity, "health");
    z_comp_health_init(health, Points);
}

static void addMood(AEntity* Entity, ZMoodType Type)
{
    ZCompMood* mood = a_entity_addComponent(Entity, "mood");
    z_comp_mood_init(mood, Type);
}

AEntity* z_entity_ship_satellite(ZCompMap* Map)
{
    AEntity* e = spawn(Map,
                       "Satellite",
                       "satellite1",
                       "satellite1",
                       "satellite1",
                       "satellite1");

    addCargo(e, Z_CARGO_TYPE_CREDS, a_random_int(3));

    addHealth(e, 15);

    return e;
}

AEntity* z_entity_ship_ship(ZCompMap* Map)
{
    AEntity* e = spawn(Map,
                       "Space Ship",
                       "ship1Up",
                       "ship1Down",
                       "ship1Left",
                       "ship1Right");

    addAiShip(e);

    addCargo(e, Z_CARGO_TYPE_CREDS, a_random_int(5));
    addCargo(e, Z_CARGO_TYPE_FUEL, a_random_int(3));

    addDamage(e, 8);

    addHealth(e, 20);

    addMood(e, Z_MOOD_GOOD);

    return e;
}
