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

#include "state_game.h"

static void playerInput(AEntity* Entity)
{
    bool acted = false;

    ZCompPosition* position = a_entity_requireComponent(Entity, "position");
    ZCompSprite* sprite = a_entity_requireComponent(Entity, "sprite");
    ZCompMood* mood = a_entity_requireComponent(Entity, "mood");
    ZMoodType moodType = z_comp_mood_getType(mood);

    int originalX, originalY;
    z_comp_position_getCoords(position, &originalX, &originalY);

    int x = originalX;
    int y = originalY;

    if(a_button_get(z_controls.up)) {
        y--;
        z_comp_sprite_setDirection(sprite, Z_SPRITE_DIRECTION_UP);
    } else if(a_button_get(z_controls.down)) {
        y++;
        z_comp_sprite_setDirection(sprite, Z_SPRITE_DIRECTION_DOWN);
    } else if(a_button_get(z_controls.left)) {
        x--;
        z_comp_sprite_setDirection(sprite, Z_SPRITE_DIRECTION_LEFT);
    } else if(a_button_get(z_controls.right)) {
        x++;
        z_comp_sprite_setDirection(sprite, Z_SPRITE_DIRECTION_RIGHT);
    }

    if(x != originalX || y != originalY) {
        if(x < 0) {
            if(z_game_moveScreen(Z_SCREEN_MOVE_LEFT)) {
                z_game_log("Moved west");
            }
        } else if(x >= Z_MAP_TILES_W) {
            if(z_game_moveScreen(Z_SCREEN_MOVE_RIGHT)) {
                z_game_log("Moved east");
            }
        } else if(y < 0) {
            if(z_game_moveScreen(Z_SCREEN_MOVE_UP)) {
                z_game_log("Moved north");
            }
        } else if(y >= Z_MAP_TILES_H) {
            if(z_game_moveScreen(Z_SCREEN_MOVE_DOWN)) {
                z_game_log("Moved south");
            }
        } else {
            ZCompMap* map = a_entity_requireComponent(z_game_getMap(), "map");

            if(z_comp_map_getTileEntity(map, x, y) == NULL) {
                // Move to this tile
                z_comp_position_setCoords(position, x, y);
                z_comp_map_setTileEntity(map, x, y, Entity);
                z_comp_map_setTileEntity(map, originalX, originalY, NULL);
            } else {
                // Interact with the entity on this tile
                AEntity* e = z_comp_map_getTileEntity(map, x, y);
                ZCompInteract* interact = a_entity_getComponent(e, "interact");

                if(interact) {
                    ZActionType action = Z_ACTION_GREET;

                    if(moodType == Z_MOOD_EVIL) {
                        action = Z_ACTION_ATTACK;
                    }

                    z_comp_interact_action(interact, Entity, action);
                }
            }

            acted = true;
        }
    }

    if(a_button_getOnce(z_controls.main)) {
        if(moodType == Z_MOOD_GOOD) {
            moodType = Z_MOOD_EVIL;
        } else if(moodType == Z_MOOD_EVIL) {
            moodType = Z_MOOD_GOOD;
        }

        z_comp_mood_setType(mood, moodType);
    }

    z_game_setWaitingForPlayer(!acted);
}

AEntity* z_entity_player_new(void)
{
    AEntity* e = a_entity_new();
    a_entity_setId(e, "playerShip");

    ZCompCargo* cargo = a_entity_addComponent(e, "cargo");
    z_comp_cargo_init(cargo);

    ZCompDamage* damage = a_entity_addComponent(e, "damage");
    z_comp_damage_init(damage, 4);

    ZCompHealth* health = a_entity_addComponent(e, "health");
    z_comp_health_init(health, 100);

    ZCompInput* input = a_entity_addComponent(e, "input");
    z_comp_input_init(input, playerInput);

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
