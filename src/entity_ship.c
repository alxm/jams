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
#include "component_position.h"
#include "component_sprite.h"

#include "state_game.h"

typedef struct ZAiContextShip {
    int x;
} ZAiContextShip;

static void shipAi(AEntity* Entity, ZAiMessageType Type, AEntity* Relevant)
{
    ZCompAi* ai = a_entity_requireComponent(Entity, "ai");
    ZAiContextShip* context = z_comp_ai_getContext(ai);

    switch(Type) {
        case Z_AI_MESSAGE_GREETED: {
            z_game_log("Hello to you too, %s", a_entity_getId(Relevant));
        } break;

        case Z_AI_MESSAGE_ATTACKED: {
            z_game_log("Ouch, %s... %d",
                       a_entity_getId(Relevant),
                       ++context->x);
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

static void addHealth(AEntity* Entity, int Points)
{
    ZCompHealth* health = a_entity_addComponent(Entity, "health");
    z_comp_health_init(health, Points);
}

static void addCargo(AEntity* Entity, ZCargoType Type, int Number)
{
    ZCompCargo* cargo = a_entity_getComponent(Entity, "cargo");

    if(cargo == NULL) {
        cargo = a_entity_addComponent(Entity, "cargo");
        z_comp_cargo_init(cargo);
    }

    z_comp_cargo_addContent(cargo, Type, Number);
}

static void addShipAi(AEntity* Entity)
{
    ZCompAi* ai = a_entity_addComponent(Entity, "ai");
    z_comp_ai_init(ai, shipAi, sizeof(ZAiContextShip));
}

AEntity* z_entity_ship_satellite(ZCompMap* Map)
{
    AEntity* e = spawn(Map,
                       "Satellite",
                       "satellite1",
                       "satellite1",
                       "satellite1",
                       "satellite1");

    addHealth(e, 15);

    addCargo(e, Z_CARGO_TYPE_CREDS, a_random_int(3));

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

    addHealth(e, 20);

    addCargo(e, Z_CARGO_TYPE_CREDS, a_random_int(5));
    addCargo(e, Z_CARGO_TYPE_FUEL, a_random_int(3));

    addShipAi(e);

    return e;
}
