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

#include "component_cargo.h"
#include "component_damage.h"
#include "component_health.h"
#include "component_interact.h"

#include "state_game.h"

void z_system_interact(AEntity* Entity)
{
    ZCompInteract* interact = a_entity_requireComponent(Entity, "interact");
    AList* pendingActions = z_comp_interact_getPending(interact);

    A_LIST_ITERATE(pendingActions, ZPendingAction*, a) {
        AEntity* actor;
        ZActionType actionType;
        z_comp_interact_getActionData(a, &actor, &actionType);

        switch(actionType) {
            case Z_ACTION_GREET: {
                z_game_setLogAction("Hello, %s!",
                                    z_comp_interact_getName(interact));
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

                        z_game_setLogAction("Destroyed %s",
                                            z_comp_interact_getName(interact));

                        if(foundCargo && actorCargo) {
                            for(ZCargoType t = Z_CARGO_TYPE_NUM; t--; ) {
                                int num = z_comp_cargo_getContent(foundCargo, t);

                                if(num > 0) {
                                    z_comp_cargo_addContent(actorCargo, t, num);
                                    z_game_setLogAction("  Plundered %d %s",
                                                        num,
                                                        z_comp_cargo_getName(t, num > 1));
                                }
                            }
                        }

                        z_game_removeEntity(Entity);
                    } else {
                        z_game_setLogAction("Attacked %s",
                                            z_comp_interact_getName(interact));
                    }
                }
            } break;
        }

        free(a);
    }

    a_list_clear(pendingActions);
}
