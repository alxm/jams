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

#include "component_input.h"
#include "component_map.h"
#include "component_position.h"
#include "component_sprite.h"

#include "system_input.h"
#include "system_map.h"
#include "system_sprite.h"

typedef struct ZGame {
    AEntity* map;
} ZGame;

static ZGame g_game;

static void playerInput(const AEntity* Entity)
{
    ZCompPosition* position = a_entity_requireComponent(Entity, "position");

    int originalX, originalY;
    z_comp_position_getCoords(position, &originalX, &originalY);

    int x = originalX;
    int y = originalY;

    if(a_button_get(z_controls.up)) {
        y--;
    } else if(a_button_get(z_controls.down)) {
        y++;
    } else if(a_button_get(z_controls.left)) {
        x--;
    } else if(a_button_get(z_controls.right)) {
        x++;
    }

    if(x != originalX || y != originalY) {
        ZCompMap* map = a_entity_requireComponent(g_game.map, "map");

        if(x >= 0 && y >= 0 && x < Z_MAP_TILES_W && y < Z_MAP_TILES_H
            && z_comp_map_getTileFreeSpace(map, x, y)) {

            z_comp_position_setCoords(position, x, y);
            z_comp_map_setTileFreeSpace(map, x, y, false);
            z_comp_map_setTileFreeSpace(map, originalX, originalY, true);
        }
    }
}

A_STATE(game)
{
    A_STATE_INIT
    {
        a_component_declare("input", z_comp_input_size(), NULL);
        a_component_declare("map", z_comp_map_size(), NULL);
        a_component_declare("position", z_comp_position_size(), NULL);
        a_component_declare("sprite", z_comp_sprite_size(), NULL);

        a_system_declare("drawMap", "map", z_system_mapDraw, NULL, false);
        a_system_declare("drawSprites", "position sprite", z_system_sprite, NULL, false);
        a_system_declare("getInputs", "input", z_system_input, NULL, false);

        a_system_tick("getInputs");
        a_system_draw("drawMap drawSprites");

        g_game.map = a_entity_new();
        ZCompMap* map = a_entity_addComponent(g_game.map, "map");
        z_comp_map_init(map);

        AEntity* player = a_entity_new();
        int x = 3, y = 3;
        z_comp_map_setTileFreeSpace(map, x, y, false);
        z_comp_position_init(a_entity_addComponent(player, "position"), x, y);
        z_comp_sprite_init(a_entity_addComponent(player, "sprite"), "playerShip");
        z_comp_input_init(a_entity_addComponent(player, "input"), playerInput);

        for(int i = 1 + a_random_int(10); i--; ) {
            AEntity* sat = a_entity_new();
            ZCompSprite* sprite = a_entity_addComponent(sat, "sprite");
            ZCompPosition* position = a_entity_addComponent(sat, "position");

            do {
                x = a_random_int(Z_MAP_TILES_W);
                y = a_random_int(Z_MAP_TILES_H);
            } while(!z_comp_map_getTileFreeSpace(map, x, y));

            z_comp_map_setTileFreeSpace(map, x, y, false);
            z_comp_position_init(position, x, y);
            z_comp_sprite_init(sprite, "satellite");
        }
    }

    A_STATE_BODY
    {
        A_STATE_LOOP;
    }
}
