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

#include "state_game.h"

void z_system_interact(AEntity* Target)
{
    ZCompAi* targetAi = a_entity_getComponent(Target, "ai");
    ZCompInteract* targetInteract = a_entity_requireComponent(Target, "interact");
    const char* targetName = z_comp_interact_getName(targetInteract);

    AEntity* actor;
    ZActionType actionType;

    while(z_comp_interact_getPending(targetInteract, &actor, &actionType)) {
        switch(actionType) {
            case Z_ACTION_GREET: {
                z_game_log("Hello, %s!", targetName);

                if(targetAi) {
                    z_comp_ai_queueMessage(targetAi,
                                           Z_AI_MESSAGE_GREETED,
                                           actor);
                }
            } break;

            case Z_ACTION_ATTACK: {
                ZCompHealth* targetHealth = a_entity_getComponent(Target, "health");

                ZCompDamage* actorDamage = a_entity_getComponent(actor, "damage");
                ZCompInteract* actorInteract = a_entity_requireComponent(actor, "interact");
                const char* actorName = z_comp_interact_getName(actorInteract);

                if(targetHealth == NULL) {
                    z_game_log("%s cannot be attacked", targetName);
                } else if(actorDamage == NULL) {
                    z_game_log("%s has no weapons to attack with", actorName);
                } else {
                    z_game_log("%s attacked %s", actorName, targetName);

                    z_comp_health_takeDamage(targetHealth,
                                             z_comp_damage_getPoints(actorDamage));


                    if(!z_comp_health_isAlive(targetHealth)) {
                        ZCompCargo* targetCargo = a_entity_getComponent(Target, "cargo");
                        ZCompCargo* actorCargo = a_entity_getComponent(actor, "cargo");

                        z_game_log("%s was destroyed", targetName);

                        if(targetCargo && actorCargo) {
                            int total = 0;

                            for(ZCargoType t = Z_CARGO_TYPE_NUM; t--; ) {
                                int num = z_comp_cargo_getNum(targetCargo, t);

                                if(num > 0) {
                                    int n = z_comp_cargo_take(actorCargo,
                                                              targetCargo,
                                                              t,
                                                              num);
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

                        if(Target == z_game_getPlayer()) {
                            z_game_log("*** THE PLAYER NEVER DIES ***");
                            z_comp_health_addPoints(targetHealth, 100);
                        } else {
                            z_game_removeEntity(Target);
                        }
                    } else {
                        if(targetAi) {
                            z_comp_ai_queueMessage(targetAi,
                                                   Z_AI_MESSAGE_ATTACKED,
                                                   actor);
                        }
                    }
                }
            } break;
        }
    }
}
