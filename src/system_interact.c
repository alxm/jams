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

void z_system_interact(AEntity* Entity)
{
    ZCompAi* ai = a_entity_getComponent(Entity, "ai");
    ZCompInteract* interact = a_entity_requireComponent(Entity, "interact");

    AEntity* actor;
    ZActionType actionType;

    while(z_comp_interact_getPending(interact, &actor, &actionType)) {
        switch(actionType) {
            case Z_ACTION_GREET: {
                z_game_log("Hello, %s!",
                           z_comp_interact_getName(interact));

                if(ai) {
                    z_comp_ai_queueMessage(ai,
                                           Z_AI_MESSAGE_GREETED,
                                           actor);
                }
            } break;

            case Z_ACTION_ATTACK: {
                ZCompHealth* health = a_entity_getComponent(Entity, "health");
                ZCompDamage* damage = a_entity_getComponent(actor, "damage");

                if(health && damage) {
                    z_comp_health_takeDamage(health,
                                             z_comp_damage_getPoints(damage));


                    if(!z_comp_health_isAlive(health)) {
                        ZCompCargo* foundCargo = a_entity_getComponent(Entity, "cargo");
                        ZCompCargo* actorCargo = a_entity_getComponent(actor, "cargo");

                        z_game_log("Destroyed %s",
                                   z_comp_interact_getName(interact));

                        if(foundCargo && actorCargo) {
                            for(ZCargoType t = Z_CARGO_TYPE_NUM; t--; ) {
                                int num = z_comp_cargo_getContent(foundCargo, t);

                                if(num > 0) {
                                    z_comp_cargo_addContent(actorCargo, t, num);
                                    z_game_log("  Plundered %d %s",
                                               num,
                                               z_comp_cargo_getName(t, num > 1));
                                }
                            }
                        }

                        z_game_removeEntity(Entity);
                    } else {
                        if(ai) {
                            z_comp_ai_queueMessage(ai,
                                                   Z_AI_MESSAGE_ATTACKED,
                                                   actor);
                        }

                        z_game_log("Attacked %s",
                                   z_comp_interact_getName(interact));
                    }
                }
            } break;
        }
    }
}
