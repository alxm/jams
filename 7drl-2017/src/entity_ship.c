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
#include "component_trade.h"

#include "entity_macros.h"

#include "state_game.h"

typedef enum {
    Z_AI_GOAL_NONE,
    Z_AI_GOAL_FLEE,
    Z_AI_GOAL_PURSUE,
} ZShipAiGoal;

typedef enum {
    Z_AI_PERSONALITY_NEUTRAL,
    Z_AI_PERSONALITY_STUBBORN,
    Z_AI_PERSONALITY_AGGRESSIVE,
    Z_AI_PERSONALITY_GREEDY,
} ZShipAiPersonality;

typedef struct ZShipAiContext {
    ZShipAiPersonality personality;
    ZShipAiGoal goal;
    int goalCounter;
    AEntity* goalContext;
} ZShipAiContext;

static void setGoal(ZShipAiContext* Context, ZShipAiGoal Goal, int Iterations, AEntity* Target)
{
    Context->goal = Goal;
    Context->goalCounter = Iterations;
    Context->goalContext = Target;

    a_entity_reference(Target);
}

static bool clearGoal(ZShipAiContext* Context)
{
    Context->goalCounter--;

    if(Context->goalCounter == 0) {
        a_entity_release(Context->goalContext);

        Context->goal = Z_AI_GOAL_NONE;
        Context->goalContext = NULL;

        return true;
    }

    return false;
}

static void attackedAiHandler(AEntity* Defender, AEntity* Attacker)
{
    ZCompAi* ai = a_entity_requireComponent(Defender, "ai");
    ZShipAiContext* context = z_comp_ai_getContext(ai);

    // A long-term goal is in progress, ignore this message
    if(context->goal != Z_AI_GOAL_NONE) {
        return;
    }

    ZCompMood* myMood = a_entity_requireComponent(Defender, "mood");
    z_comp_mood_setType(myMood, Z_MOOD_EVIL);

    ZCompHealth* myHealth = a_entity_requireComponent(Defender, "health");
    int myHealthPoints, myHealthMax;
    z_comp_health_getStats(myHealth, &myHealthPoints, &myHealthMax);

    ZCompHealth* itsHealth = a_entity_requireComponent(Attacker, "health");
    int itsHealthPoints, itsHealthMax;
    z_comp_health_getStats(itsHealth, &itsHealthPoints, &itsHealthMax);

    bool amIWeak = myHealthPoints < myHealthMax / 4;
    bool isItWeak = itsHealthPoints < itsHealthMax / 3;

    switch(context->personality) {
        case Z_AI_PERSONALITY_GREEDY: {
            // Merchants hold grudges and won't offer repairs
            ZCompTrade* t = a_entity_requireComponent(Defender, "trade");
            z_comp_trade_setDoesRepairs(t, false);
        }

        case Z_AI_PERSONALITY_NEUTRAL: {
            if(amIWeak) {
                // Flee
                setGoal(context, Z_AI_GOAL_FLEE, 5, Attacker);
                z_comp_mood_setType(myMood, Z_MOOD_GOOD);
            } else if(isItWeak) {
                // Retaliate several times
                setGoal(context, Z_AI_GOAL_PURSUE, 3, Attacker);
            } else {
                // Retaliate once
                setGoal(context, Z_AI_GOAL_PURSUE, 1, Attacker);
            }
        } break;

        case Z_AI_PERSONALITY_STUBBORN: {
            if(isItWeak) {
                // Retaliate several times
                setGoal(context, Z_AI_GOAL_PURSUE, 3, Attacker);
            } else if(amIWeak) {
                // Flee
                setGoal(context, Z_AI_GOAL_FLEE, 5, Attacker);
                z_comp_mood_setType(myMood, Z_MOOD_GOOD);
            } else {
                // Retaliate once
                setGoal(context, Z_AI_GOAL_PURSUE, 1, Attacker);
            }
        } break;

        case Z_AI_PERSONALITY_AGGRESSIVE: {
            // Retaliate several times regardless of anything
            setGoal(context, Z_AI_GOAL_PURSUE, 5, Attacker);
        } break;
    }
}

static void merchantHandleGreeting(AEntity* Merchant, AEntity* Customer)
{
    ZCompAi* ai = a_entity_requireComponent(Merchant, "ai");
    ZShipAiContext* context = z_comp_ai_getContext(ai);

    // A long-term goal is in progress, ignore this message
    if(context->goal != Z_AI_GOAL_NONE) {
        return;
    }

    ZCompInteract* itsInteract = a_entity_requireComponent(Customer, "interact");
    const char* itsName = z_comp_interact_getName(itsInteract);

    z_game_log("Welcome to my enterprise, %s", itsName);
    z_game_tradeOn(Merchant);
}

static void shipTickAi(AEntity* Entity, ZCompAi* Ai)
{
    ZShipAiContext* context = z_comp_ai_getContext(Ai);

    if(context->goal == Z_AI_GOAL_NONE) {
        if(context->personality == Z_AI_PERSONALITY_AGGRESSIVE) {
            // Not doing anything, pursue player
            setGoal(context, Z_AI_GOAL_PURSUE, 10, z_game_getPlayer());
        } else {
            return;
        }
    }

    ZCompMood* myMood = a_entity_requireComponent(Entity, "mood");
    ZCompPosition* myPos = a_entity_requireComponent(Entity, "position");
    ZCompPosition* itsPos = a_entity_requireComponent(context->goalContext, "position");

    int myX, myY;
    z_comp_position_getCoords(myPos, &myX, &myY);

    int itsX, itsY;
    z_comp_position_getCoords(itsPos, &itsX, &itsY);

    switch(context->goal) {
        case Z_AI_GOAL_FLEE: {
            bool iMoved = false;

            if(myX < itsX) {
                iMoved = z_entity_macro_move(Entity, Z_MOVE_LEFT);
            } else if(myX > itsX) {
                iMoved = z_entity_macro_move(Entity, Z_MOVE_RIGHT);
            }

            if(iMoved) {
                break;
            }

            if(myY < itsY) {
                iMoved = z_entity_macro_move(Entity, Z_MOVE_UP);
            } else if(myY > itsY) {
                iMoved = z_entity_macro_move(Entity, Z_MOVE_DOWN);
            }

            if(iMoved) {
                break;
            }

            if(myX == itsX) {
                ZMove randDir = Z_MOVE_LEFT + a_random_getInt(2);
                iMoved = z_entity_macro_move(Entity, randDir);
            }

            if(iMoved) {
                break;
            }

            if(myY == itsY) {
                ZMove randDir = Z_MOVE_UP + a_random_getInt(2);
                iMoved = z_entity_macro_move(Entity, randDir);
            }

            if(iMoved) {
                break;
            }

            // Could not run away, attack a few times instead
            z_comp_mood_setType(myMood, Z_MOOD_EVIL);
            context->goal = Z_AI_GOAL_PURSUE;
            context->goalCounter += 2;
        } break;

        case Z_AI_GOAL_PURSUE: {
            if(myX < itsX) {
                z_entity_macro_move(Entity, Z_MOVE_RIGHT);
            } else if(myX > itsX) {
                z_entity_macro_move(Entity, Z_MOVE_LEFT);
            } else if(myY < itsY) {
                z_entity_macro_move(Entity, Z_MOVE_DOWN);
            } else if(myY > itsY) {
                z_entity_macro_move(Entity, Z_MOVE_UP);
            }
        } break;

        default: break;
    }

    if(clearGoal(context)) {
        if(context->personality != Z_AI_PERSONALITY_AGGRESSIVE) {
            z_comp_mood_setType(myMood, Z_MOOD_GOOD);
        }
    }
}

static void addAiShip(AEntity* Entity, ZShipAiPersonality Personality)
{
    ZCompAi* ai = a_entity_addComponent(Entity, "ai");
    z_comp_ai_init(ai, shipTickAi, sizeof(ZShipAiContext));

    ZShipAiContext* context = z_comp_ai_getContext(ai);
    context->personality = Personality;

    a_entity_setMessageHandler(Entity, "attacked", attackedAiHandler);
}

static void addCargo(AEntity* Entity, ZCargoType Type, int Number)
{
    ZCompCargo* cargo = a_entity_getComponent(Entity, "cargo");

    if(cargo == NULL) {
        cargo = a_entity_addComponent(Entity, "cargo");
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
    AEntity* e = z_entity_macro_spawn(Map,
                                      "Satellite",
                                      "satellite1",
                                      "satellite1",
                                      "satellite1",
                                      "satellite1");

    a_entity_setMessageHandler(e, "greeting", z_entity_macro_handleGreeting);
    a_entity_setMessageHandler(e, "aggression", z_entity_macro_handleAttack);

    addCargo(e, Z_CARGO_TYPE_CREDS, a_random_getInt(3));

    addHealth(e, 15);

    return e;
}

AEntity* z_entity_ship_neutralShip(ZCompMap* Map)
{
    AEntity* e = z_entity_macro_spawn(Map,
                                      "Neutral Ship",
                                      "ship1Up",
                                      "ship1Down",
                                      "ship1Left",
                                      "ship1Right");

    a_entity_setMessageHandler(e, "greeting", z_entity_macro_handleGreeting);
    a_entity_setMessageHandler(e, "aggression", z_entity_macro_handleAttack);

    addAiShip(e, Z_AI_PERSONALITY_NEUTRAL);
    addMood(e, Z_MOOD_GOOD);

    addDamage(e, 1);
    addHealth(e, 15);

    addCargo(e, Z_CARGO_TYPE_CREDS, 1 + a_random_getInt(3));
    addCargo(e, Z_CARGO_TYPE_FUEL, a_random_getInt(3));

    return e;
}

AEntity* z_entity_ship_patrolShip(ZCompMap* Map)
{
    AEntity* e = z_entity_macro_spawn(Map,
                                      "Patrol Ship",
                                      "ship2Up",
                                      "ship2Down",
                                      "ship2Left",
                                      "ship2Right");

    a_entity_setMessageHandler(e, "greeting", z_entity_macro_handleGreeting);
    a_entity_setMessageHandler(e, "aggression", z_entity_macro_handleAttack);

    addAiShip(e, Z_AI_PERSONALITY_STUBBORN);
    addMood(e, Z_MOOD_EVIL);

    addDamage(e, 2);
    addHealth(e, 25);

    addCargo(e, Z_CARGO_TYPE_CREDS, 2 + a_random_getInt(4));
    addCargo(e, Z_CARGO_TYPE_FUEL, 1 + a_random_getInt(3));

    return e;
}

AEntity* z_entity_ship_fighterShip(ZCompMap* Map)
{
    AEntity* e = z_entity_macro_spawn(Map,
                                      "Fighter Ship",
                                      "ship2Up",
                                      "ship2Down",
                                      "ship2Left",
                                      "ship2Right");

    a_entity_setMessageHandler(e, "greeting", z_entity_macro_handleGreeting);
    a_entity_setMessageHandler(e, "aggression", z_entity_macro_handleAttack);

    addAiShip(e, Z_AI_PERSONALITY_AGGRESSIVE);
    addMood(e, Z_MOOD_EVIL);

    addDamage(e, 3);
    addHealth(e, 30);

    addCargo(e, Z_CARGO_TYPE_CREDS, 0);

    return e;
}

AEntity* z_entity_ship_merchantShip(ZCompMap* Map)
{
    AEntity* e = z_entity_macro_spawn(Map,
                                      "Merchant Ship",
                                      "tradingShipUp",
                                      "tradingShipDown",
                                      "tradingShipLeft",
                                      "tradingShipRight");

    a_entity_setMessageHandler(e, "greeting", merchantHandleGreeting);
    a_entity_setMessageHandler(e, "aggression", z_entity_macro_handleAttack);

    addAiShip(e, Z_AI_PERSONALITY_GREEDY);
    addMood(e, Z_MOOD_GOOD);

    addDamage(e, 5);
    addHealth(e, 50);

    addCargo(e, Z_CARGO_TYPE_CREDS, 1000 + a_random_getInt(1000));
    addCargo(e, Z_CARGO_TYPE_FUEL, a_random_getInt(100));
    addCargo(e, Z_CARGO_TYPE_MINERALS, a_random_getInt(50));

    ZCompTrade* trade = a_entity_addComponent(e, "trade");
    z_comp_trade_init(trade);

    return e;
}
