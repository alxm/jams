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

#define Z_UNIVERSE_DIM 256

typedef enum {
    Z_SCREEN_MOVE_NONE,
    Z_SCREEN_MOVE_LEFT,
    Z_SCREEN_MOVE_RIGHT,
    Z_SCREEN_MOVE_UP,
    Z_SCREEN_MOVE_DOWN
} ZScreenMove;

typedef struct ZGame {
    unsigned initialSeed;
    unsigned universeX, universeY;
    ZScreenMove moveAction;
    int lastPlayerX, lastPlayerY;
    AEntity* map;
    ASprite* oldMapScreen;
    ASprite* newMapScreen;
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
        if(x < 0) {
            if(g_game.universeX > 0) {
                g_game.universeX--;
                g_game.moveAction = Z_SCREEN_MOVE_LEFT;
                g_game.lastPlayerY = y;
            }
        } else if(x >= Z_MAP_TILES_W) {
            if(g_game.universeX < Z_UNIVERSE_DIM - 1) {
                g_game.universeX++;
                g_game.moveAction = Z_SCREEN_MOVE_RIGHT;
                g_game.lastPlayerY = y;
            }
        } else if(y < 0) {
            if(g_game.universeY > 0) {
                g_game.universeY--;
                g_game.moveAction = Z_SCREEN_MOVE_UP;
                g_game.lastPlayerX = x;
            }
        } else if(y >= Z_MAP_TILES_H) {
            if(g_game.universeY < Z_UNIVERSE_DIM - 1) {
                g_game.universeY++;
                g_game.moveAction = Z_SCREEN_MOVE_DOWN;
                g_game.lastPlayerX = x;
            }
        } else {
            ZCompMap* map = a_entity_requireComponent(g_game.map, "map");

            if(x >= 0 && y >= 0 && x < Z_MAP_TILES_W && y < Z_MAP_TILES_H
                && z_comp_map_getTileFreeSpace(map, x, y)) {

                z_comp_position_setCoords(position, x, y);
                z_comp_map_setTileFreeSpace(map, x, y, false);
                z_comp_map_setTileFreeSpace(map, originalX, originalY, true);
            }
        }

        if(g_game.moveAction != Z_SCREEN_MOVE_NONE) {
            a_state_pop();
            a_state_push("playGame");
        }
    }
}

A_STATE(newGame)
{
    A_STATE_INIT
    {
        g_game.initialSeed = a_random_getSeed();
        g_game.universeX = Z_UNIVERSE_DIM / 2;
        g_game.universeY = Z_UNIVERSE_DIM / 2;
        g_game.moveAction = Z_SCREEN_MOVE_NONE;
        g_game.oldMapScreen = a_sprite_blank(Z_MAP_TILES_W * Z_MAP_TILE_DIM,
                                             Z_MAP_TILES_H * Z_MAP_TILE_DIM,
                                             false);
        g_game.newMapScreen = a_sprite_blank(Z_MAP_TILES_W * Z_MAP_TILE_DIM,
                                             Z_MAP_TILES_H * Z_MAP_TILE_DIM,
                                             false);
    }

    A_STATE_BODY
    {
        a_state_push("playGame");
    }
}

A_STATE(playGame)
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

        // This screen's unique seed
        a_random_setSeed(g_game.initialSeed
                         + g_game.universeY * Z_UNIVERSE_DIM
                         + g_game.universeX);

        g_game.map = a_entity_new();
        ZCompMap* map = a_entity_addComponent(g_game.map, "map");
        z_comp_map_init(map);

        int x, y;

        switch(g_game.moveAction) {
            case Z_SCREEN_MOVE_NONE: {
                x = Z_MAP_TILES_W / 2;
                y = Z_MAP_TILES_H / 2;
            } break;

            case Z_SCREEN_MOVE_LEFT: {
                x = Z_MAP_TILES_W - 1;
                y = g_game.lastPlayerY;
            } break;

            case Z_SCREEN_MOVE_RIGHT: {
                x = 0;
                y = g_game.lastPlayerY;
            } break;

            case Z_SCREEN_MOVE_UP: {
                x = g_game.lastPlayerX;
                y = Z_MAP_TILES_H - 1;
            } break;

            case Z_SCREEN_MOVE_DOWN: {
                x = g_game.lastPlayerX;
                y = 0;
            } break;
        }

        if(g_game.moveAction != Z_SCREEN_MOVE_NONE) {
            // Draw the new map and feed it to the nextScreen transition state
            a_screen_setTargetSprite(g_game.newMapScreen);
            z_system_mapDraw(g_game.map);
            a_screen_resetTarget();

            a_state_push("nextScreen");
        }

        AEntity* player = a_entity_new();
        z_comp_map_setTileFreeSpace(map, x, y, false);
        z_comp_position_init(a_entity_addComponent(player, "position"), x, y);
        z_comp_sprite_init(a_entity_addComponent(player, "sprite"), "playerShip");
        z_comp_input_init(a_entity_addComponent(player, "input"), playerInput);

        for(int i = 1 + a_random_int(10); i--; ) {
            AEntity* sat = a_entity_new();
            ZCompSprite* sprite = a_entity_addComponent(sat, "sprite");
            ZCompPosition* position = a_entity_addComponent(sat, "position");

            do {
                x = 1 + a_random_int(Z_MAP_TILES_W - 2);
                y = 1 + a_random_int(Z_MAP_TILES_H - 2);
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

A_STATE(nextScreen)
{
    A_STATE_INIT
    {
        // Capture the old map
        a_screen_copyPart(a_sprite_pixels(g_game.oldMapScreen),
                          0,
                          0,
                          a_sprite_width(g_game.oldMapScreen),
                          a_sprite_height(g_game.oldMapScreen));
    }

    A_STATE_BODY
    {
        int mapSpriteW = a_sprite_width(g_game.oldMapScreen);
        int mapSpriteH = a_sprite_height(g_game.oldMapScreen);

        int xInc = 0, yInc = 0;
        int xStart = 0, yStart = 0;
        int numMoves = 0;
        int movesPerFrame = Z_MAP_TILE_DIM / 4;

        switch(g_game.moveAction) {
            case Z_SCREEN_MOVE_LEFT: {
                xInc = 1;
                xStart = -mapSpriteW;
                numMoves = mapSpriteW;
            } break;

            case Z_SCREEN_MOVE_RIGHT: {
                xInc = -1;
                xStart = mapSpriteW;
                numMoves = mapSpriteW;
            } break;

            case Z_SCREEN_MOVE_UP: {
                yInc = 1;
                yStart = -mapSpriteH;
                numMoves = mapSpriteH;
            } break;

            case Z_SCREEN_MOVE_DOWN: {
                yInc = -1;
                yStart = mapSpriteH;
                numMoves = mapSpriteH;
            } break;

            default: break;
        }

        a_screen_setClip(0, 0, mapSpriteW, mapSpriteH);

        A_STATE_LOOP
        {
            a_sprite_blit(g_game.newMapScreen, xStart, yStart);
            a_sprite_blit(g_game.oldMapScreen,
                          xStart + xInc * mapSpriteW,
                          yStart + yInc * mapSpriteH);

            xStart += xInc * movesPerFrame;
            yStart += yInc * movesPerFrame;
            numMoves -= movesPerFrame;

            if(numMoves <= 0) {
                a_state_pop();
            }
        }

        a_screen_resetClip();
    }

    A_STATE_FREE
    {
        g_game.moveAction = Z_SCREEN_MOVE_NONE;
    }
}
