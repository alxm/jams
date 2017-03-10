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

#include "component_interact.h"
#include "component_map.h"
#include "component_mood.h"
#include "component_position.h"
#include "component_sprite.h"

#include "entity_macros.h"

#include "state_game.h"

bool z_entity_macro_move(AEntity* Entity, ZMove Direction)
{
    bool acted = false;

    ZCompMood* mood = a_entity_requireComponent(Entity, "mood");
    ZCompPosition* position = a_entity_requireComponent(Entity, "position");
    ZCompSprite* sprite = a_entity_requireComponent(Entity, "sprite");

    int originalX, originalY;
    z_comp_position_getCoords(position, &originalX, &originalY);

    int x = originalX;
    int y = originalY;

    switch(Direction) {
        case Z_MOVE_UP: {
            y--;
            z_comp_sprite_setDirection(sprite, Z_SPRITE_DIRECTION_UP);
        } break;

        case Z_MOVE_DOWN: {
            y++;
            z_comp_sprite_setDirection(sprite, Z_SPRITE_DIRECTION_DOWN);
        } break;

        case Z_MOVE_LEFT: {
            x--;
            z_comp_sprite_setDirection(sprite, Z_SPRITE_DIRECTION_LEFT);
        } break;

        case Z_MOVE_RIGHT: {
            x++;
            z_comp_sprite_setDirection(sprite, Z_SPRITE_DIRECTION_RIGHT);
        } break;
    }

    // Only the player may move to a different screen
    AEntity* player = z_game_getPlayer();

    if(x < 0) {
        if(Entity == player && z_game_moveScreen(Z_SCREEN_MOVE_LEFT)) {
            z_game_log("Moved west");
        }
    } else if(x >= Z_MAP_TILES_W) {
        if(Entity == player && z_game_moveScreen(Z_SCREEN_MOVE_RIGHT)) {
            z_game_log("Moved east");
        }
    } else if(y < 0) {
        if(Entity == player && z_game_moveScreen(Z_SCREEN_MOVE_UP)) {
            z_game_log("Moved north");
        }
    } else if(y >= Z_MAP_TILES_H) {
        if(Entity == player && z_game_moveScreen(Z_SCREEN_MOVE_DOWN)) {
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
                ZMoodType moodType = z_comp_mood_getType(mood);

                if(moodType == Z_MOOD_EVIL) {
                    action = Z_ACTION_ATTACK;
                }

                z_comp_interact_action(interact, Entity, action);
            }
        }

        acted = true;
    }

    return acted;
}
