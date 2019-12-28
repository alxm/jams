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

#include "util_controls.h"
#include "util_graphics.h"

#include "component_cargo.h"
#include "component_damage.h"
#include "component_health.h"
#include "component_input.h"
#include "component_interact.h"
#include "component_map.h"
#include "component_mood.h"
#include "component_position.h"
#include "component_sprite.h"

#include "entity_macros.h"

#include "state_game.h"

static void playerInput(AEntity* Entity)
{
    if(!z_game_isPlayerTurn()) {
        return;
    }

    bool acted = false;

    if(a_button_getPressed(z_controls.up)) {
        acted = z_entity_macro_move(Entity, Z_MOVE_UP);
    } else if(a_button_getPressed(z_controls.down)) {
        acted = z_entity_macro_move(Entity, Z_MOVE_DOWN);
    } else if(a_button_getPressed(z_controls.left)) {
        acted = z_entity_macro_move(Entity, Z_MOVE_LEFT);
    } else if(a_button_getPressed(z_controls.right)) {
        acted = z_entity_macro_move(Entity, Z_MOVE_RIGHT);
    }

    if(a_button_getPressedOnce(z_controls.primary)) {
        ZCompMood* mood = a_entity_requireComponent(Entity, "mood");
        ZMoodType moodType = z_comp_mood_getType(mood);

        if(moodType == Z_MOOD_GOOD) {
            moodType = Z_MOOD_EVIL;
        } else if(moodType == Z_MOOD_EVIL) {
            moodType = Z_MOOD_GOOD;
        }

        z_comp_mood_setType(mood, moodType);
    }

    if(acted) {
        z_game_playerActed();
    }
}

AEntity* z_entity_player_new(void)
{
    AEntity* e = a_entity_new("player", NULL);

    a_entity_setMessageHandler(e, "aggression", z_entity_macro_handleAttack);

    ZCompCargo* cargo = a_entity_addComponent(e, "cargo");
    z_comp_cargo_add(cargo, Z_CARGO_TYPE_CREDS, 2);

    ZCompDamage* damage = a_entity_addComponent(e, "damage");
    z_comp_damage_init(damage, 5);

    ZCompHealth* health = a_entity_addComponent(e, "health");
    z_comp_health_init(health, 20);

    ZCompInput* input = a_entity_addComponent(e, "input");
    z_comp_input_init(input, playerInput);

    ZCompInteract* interact = a_entity_addComponent(e, "interact");
    z_comp_interact_init(interact, "Player");

    ZCompMood* mood = a_entity_addComponent(e, "mood");
    z_comp_mood_init(mood, Z_MOOD_GOOD);

    ZCompPosition* position = a_entity_addComponent(e, "position");
    z_comp_position_init(position, Z_MAP_TILES_W / 2, Z_MAP_TILES_H / 2);

    ZCompSprite* sprite = a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite,
                       "playerShipUp",
                       "playerShipDown",
                       "playerShipLeft",
                       "playerShipRight");

    a_entity_addComponent(e, "hud");

    return e;
}
