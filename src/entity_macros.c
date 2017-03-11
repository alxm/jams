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

AEntity* z_entity_macro_spawn(ZCompMap* Map, const char* Name, const char* Up, const char* Down, const char* Left, const char* Right)
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
            acted = true;
        } else {
            // Interact with the entity on this tile
            AEntity* target = z_comp_map_getTileEntity(map, x, y);
            ZCompInteract* targetInteract = a_entity_getComponent(target, "interact");

            if(targetInteract) {
                ZMoodType moodType = z_comp_mood_getType(mood);

                if(moodType == Z_MOOD_EVIL) {
                    z_comp_interact_action(targetInteract, Entity, Z_INTERACTION_AGGRESSIVE);
                    acted = true;
                } else if(Entity == player) { // No need for AI-AI greeting :-)
                    z_comp_interact_action(targetInteract, Entity, Z_INTERACTION_BENEVOLENT);
                    acted = true;
                }
            }
        }
    }

    return acted;
}
