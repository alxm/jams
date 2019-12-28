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
#include "component_interact.h"
#include "component_map.h"

#include "entity_macros.h"

#include "state_game.h"

static void miningHandler(AEntity* Entity, AEntity* Sender)
{
    ZCompCargo* actorCargo = a_entity_requireComponent(Sender, "cargo");
    ZCompInteract* actorInter = a_entity_requireComponent(Sender, "interact");

    ZCompCargo* targetCargo = a_entity_requireComponent(Entity, "cargo");
    ZCompInteract* targetInter = a_entity_requireComponent(Entity, "interact");

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

AEntity* z_entity_asteroid_new(ZCompMap* Map)
{
    AEntity* e = z_entity_macro_spawn(Map,
                                      "Asteroid",
                                      "asteroid",
                                      "asteroid",
                                      "asteroid",
                                      "asteroid");

    ZCompCargo* cargo = a_entity_addComponent(e, "cargo");
    z_comp_cargo_add(cargo, Z_CARGO_TYPE_MINERALS, 1 + a_random_getInt(4));

    a_entity_setMessageHandler(e, "greeting", z_entity_macro_handleGreeting);
    a_entity_setMessageHandler(e, "aggression", miningHandler);

    return e;
}
