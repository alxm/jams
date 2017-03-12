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

#include "component_ai.h"
#include "component_cargo.h"
#include "component_damage.h"
#include "component_health.h"
#include "component_interact.h"
#include "component_position.h"
#include "component_sprite.h"

#include "state_game.h"

static void attack(AEntity* Actor, AEntity* Target)
{
    ZCompCargo* actorCargo = a_entity_getComponent(Actor, "cargo");
    ZCompDamage* actorDamage = a_entity_requireComponent(Actor, "damage");
    ZCompInteract* actorInter = a_entity_requireComponent(Actor, "interact");

    ZCompCargo* targetCargo = a_entity_getComponent(Target, "cargo");
    ZCompHealth* targetHealth = a_entity_requireComponent(Target, "health");
    ZCompInteract* targetInter = a_entity_requireComponent(Target, "interact");

    const char* actorName = z_comp_interact_getName(actorInter);
    const char* targetName = z_comp_interact_getName(targetInter);

    int actorDamagePoints = z_comp_damage_getPoints(actorDamage);

    z_game_log("%s attacked %s", actorName, targetName);
    int dmg = z_comp_health_takeDamage(targetHealth, actorDamagePoints);
    z_game_log("  Dealt %d damage", dmg);

    if(z_comp_health_isAlive(targetHealth)) {
        return;
    }

    z_game_log("%s was destroyed", targetName);

    if(targetCargo && actorCargo) {
        int total = 0;

        for(ZCargoType t = Z_CARGO_TYPE_NUM; t--; ) {
            int num = z_comp_cargo_getNum(targetCargo, t);

            if(num > 0) {
                int n = z_comp_cargo_take(actorCargo, targetCargo, t, num);
                total += n;

                z_game_log("  %s plundered %d %s",
                           actorName,
                           n,
                           z_comp_cargo_getName(t, n > 1));
            }
        }

        if(total == 0) {
            z_game_log("  %s had no cargo", targetName);
        }
    }

    z_game_removeEntity(Target);
}

static void mine(AEntity* Actor, AEntity* Target)
{
    ZCompCargo* actorCargo = a_entity_requireComponent(Actor, "cargo");
    ZCompInteract* actorInter = a_entity_requireComponent(Actor, "interact");

    ZCompCargo* targetCargo = a_entity_requireComponent(Target, "cargo");
    ZCompInteract* targetInter = a_entity_requireComponent(Target, "interact");

    const char* actorName = z_comp_interact_getName(actorInter);
    const char* targetName = z_comp_interact_getName(targetInter);

    int n = z_comp_cargo_take(actorCargo,
                              targetCargo,
                              Z_CARGO_TYPE_MINERALS,
                              1);

    z_game_log("%s mined %s", actorName, targetName);

    if(n > 0) {
        z_game_log("  Extracted %d %s",
                   n,
                   z_comp_cargo_getName(Z_CARGO_TYPE_MINERALS, n > 1));
    } else {
        z_game_log("  %s is depleted", targetName);
    }
}

void z_system_interact(AEntity* Target)
{
    ZCompAi* targetAi = a_entity_getComponent(Target, "ai");
    ZCompInteract* targetInter = a_entity_requireComponent(Target, "interact");

    AEntity* actor;
    ZInteractionType type;
    int lastActorX = -1, lastActorY;

    while(z_comp_interact_getPending(targetInter, &actor, &type)) {
        if(a_entity_isRemoved(actor)) {
            // Actor entity was killed just after it queued this interaction
            continue;
        }

        switch(type) {
            case Z_INTERACTION_BENEVOLENT: {
                if(actor == z_game_getPlayer()) {
                    z_game_log("Hello, %s!",
                               z_comp_interact_getName(targetInter));
                }
            } break;

            case Z_INTERACTION_AGGRESSIVE: {
                if(a_entity_hasComponent(Target, "health")
                    && a_entity_hasComponent(actor, "damage")) {

                    attack(actor, Target);
                } else if(a_entity_hasComponent(Target, "cargo")
                    && a_entity_hasComponent(actor, "cargo")) {

                    mine(actor, Target);
                }
            } break;
        }

        if(targetAi) {
            z_comp_ai_queueMessage(targetAi, type, actor);
        }

        ZCompPosition* actorPos = a_entity_requireComponent(actor, "position");
        z_comp_position_getCoords(actorPos, &lastActorX, &lastActorY);
    }

    if(lastActorX != -1) {
        ZCompPosition* targetP = a_entity_requireComponent(Target, "position");
        ZCompSprite* targetSprite = a_entity_requireComponent(Target, "sprite");

        int targetX, targetY;
        z_comp_position_getCoords(targetP, &targetX, &targetY);

        if(lastActorX < targetX) {
            z_comp_sprite_setDirection(targetSprite, Z_SPRITE_DIRECTION_LEFT);
        } else if(lastActorX > targetX) {
            z_comp_sprite_setDirection(targetSprite, Z_SPRITE_DIRECTION_RIGHT);
        } else if(lastActorY < targetY) {
            z_comp_sprite_setDirection(targetSprite, Z_SPRITE_DIRECTION_UP);
        } else if(lastActorY > targetY) {
            z_comp_sprite_setDirection(targetSprite, Z_SPRITE_DIRECTION_DOWN);
        }
    }
}
