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

typedef enum {
    Z_AI_GOAL_NONE,
    Z_AI_GOAL_FLEE,
    Z_AI_GOAL_PURSUE,
} ZShipAiGoal;

typedef struct ZShipAiContext {
    int annoyedCounter;
    ZShipAiGoal goal;
    int goalCounter;
    AEntity* goalContext;
} ZShipAiContext;

static void shipMessageAi(AEntity* Entity, ZCompAi* Ai, ZAiMessageType Type, AEntity* Relevant)
{
    ZShipAiContext* context = z_comp_ai_getContext(Ai);

    // A long-term goal is in progress, ignore this message
    if(context->goal != Z_AI_GOAL_NONE) {
        return;
    }

    ZCompMood* mood = a_entity_requireComponent(Entity, "mood");
    ZMoodType currentMood = z_comp_mood_getType(mood);

    ZCompInteract* actorInteract = a_entity_requireComponent(Relevant,
                                                             "interact");
    const char* actorName = z_comp_interact_getName(actorInteract);

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
            // Retaliate
            context->annoyedCounter++;
            z_comp_mood_setType(mood, Z_MOOD_EVIL);

            ZCompHealth* myHealth = a_entity_requireComponent(Entity, "health");
            int myHealthPoints, myHealthMax;
            z_comp_health_getStats(myHealth, &myHealthPoints, &myHealthMax);

            ZCompHealth* enHealth = a_entity_requireComponent(Relevant, "health");
            int enHealthPoints, enHealthMax;
            z_comp_health_getStats(enHealth, &enHealthPoints, &enHealthMax);

            bool amWeak = myHealthPoints < myHealthMax / 2;
            bool isWeak = enHealthPoints < enHealthMax / 2;

            if(isWeak) {
                // Pursue
                context->goal = Z_AI_GOAL_PURSUE;
                context->goalCounter = 3;
            } else if(amWeak) {
                // Flee
                z_comp_mood_setType(mood, Z_MOOD_GOOD);
                context->goal = Z_AI_GOAL_FLEE;
                context->goalCounter = 5;
            } else {
                // Retaliate once right now
                context->goal = Z_AI_GOAL_PURSUE;
                context->goalCounter = 1;
            }

            context->goalContext = Relevant;
            a_entity_reference(Relevant);
        } break;
    }
}

static void shipTickAi(AEntity* Entity, ZCompAi* Ai)
{
    ZShipAiContext* context = z_comp_ai_getContext(Ai);

    if(context->goal == Z_AI_GOAL_NONE) {
        return;
    }

    ZCompMood* myMood = a_entity_requireComponent(Entity, "mood");
    ZCompPosition* myPos = a_entity_requireComponent(Entity, "position");
    ZCompPosition* actorPos = a_entity_requireComponent(context->goalContext,
                                                        "position");

    int myX, myY;
    z_comp_position_getCoords(myPos, &myX, &myY);

    int actorX, actorY;
    z_comp_position_getCoords(actorPos, &actorX, &actorY);

    switch(context->goal) {
        case Z_AI_GOAL_FLEE: {
            bool moved = false;

            if(myX < actorX) {
                moved = z_entity_macro_move(Entity, Z_MOVE_LEFT);
            } else if(myX > actorX) {
                moved = z_entity_macro_move(Entity, Z_MOVE_RIGHT);
            }

            if(moved) {
                break;
            }

            if(myY < actorY) {
                moved = z_entity_macro_move(Entity, Z_MOVE_UP);
            } else if(myY > actorY) {
                moved = z_entity_macro_move(Entity, Z_MOVE_DOWN);
            }

            if(moved) {
                break;
            }

            if(myX == actorX) {
                ZMove randDir = Z_MOVE_LEFT + a_random_int(2);
                moved = z_entity_macro_move(Entity, randDir);
            }

            if(moved) {
                break;
            }

            if(myY == actorY) {
                ZMove randDir = Z_MOVE_UP + a_random_int(2);
                moved = z_entity_macro_move(Entity, randDir);
            }

            if(moved) {
                break;
            }

            // Could not run away, attack a few times instead
            z_comp_mood_setType(myMood, Z_MOOD_EVIL);
            context->goal = Z_AI_GOAL_PURSUE;
            context->goalCounter += 2;
        } break;

        case Z_AI_GOAL_PURSUE: {
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

        default: break;
    }

    if(--context->goalCounter == 0) {
        a_entity_release(context->goalContext);
        context->goal = Z_AI_GOAL_NONE;
        z_comp_mood_setType(myMood, Z_MOOD_GOOD);
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
    z_comp_ai_init(ai, shipMessageAi, shipTickAi, sizeof(ZShipAiContext));
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
