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

#include "component_damage.h"
#include "component_health.h"
#include "component_input.h"
#include "component_interact.h"
#include "component_map.h"
#include "component_mood.h"
#include "component_position.h"
#include "component_sprite.h"

#include "system_input.h"
#include "system_interact.h"
#include "system_hud.h"
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
    ZMoodType lastPlayerMood;
    AEntity* playerShip;
    AEntity* map;
    ASprite* oldMapScreen;
    ASprite* newMapScreen;
    AList* lines;
} ZGame;

static ZGame g_game;

void z_game_getUniverseCoords(unsigned* X, unsigned* Y)
{
    *X = g_game.universeX;
    *Y = g_game.universeY;
}

AEntity* z_game_getPlayer(void)
{
    return g_game.playerShip;
}

void z_game_setLogAction(const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    int needed = vsnprintf(NULL, 0, Format, args);

    va_end(args);
    va_start(args, Format);

    if(needed > 0) {
        size_t size = (size_t)needed + 1;
        char* buffer = a_mem_malloc(size);
        needed = vsnprintf(buffer, size, Format, args);

        if(needed > 0 && (size_t)needed < size) {
            a_list_addLast(g_game.lines, buffer);
            a_out_textf(buffer);

            if(a_list_size(g_game.lines) > 21) {
                free(a_list_pop(g_game.lines));
            }
        } else {
            free(buffer);
        }
    }

    va_end(args);
}

AList* z_game_getLogLines(void)
{
    return g_game.lines;
}

void z_game_removeEntity(AEntity* Entity)
{
    ZCompPosition* position = a_entity_getComponent(Entity, "position");

    if(position) {
        int x, y;
        z_comp_position_getCoords(position, &x, &y);

        ZCompMap* map = a_entity_requireComponent(g_game.map, "map");
        z_comp_map_setTileEntity(map, x, y, NULL);
    }

    a_entity_remove(Entity);
}

static void playerInput(AEntity* Entity)
{
    ZCompPosition* position = a_entity_requireComponent(Entity, "position");

    ZCompMood* mood = a_entity_requireComponent(Entity, "mood");
    ZMoodType moodType = z_comp_mood_getType(mood);

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
                z_game_setLogAction("Moved west");
            }
        } else if(x >= Z_MAP_TILES_W) {
            if(g_game.universeX < Z_UNIVERSE_DIM - 1) {
                g_game.universeX++;
                g_game.moveAction = Z_SCREEN_MOVE_RIGHT;
                g_game.lastPlayerY = y;
                z_game_setLogAction("Moved east");
            }
        } else if(y < 0) {
            if(g_game.universeY > 0) {
                g_game.universeY--;
                g_game.moveAction = Z_SCREEN_MOVE_UP;
                g_game.lastPlayerX = x;
                z_game_setLogAction("Moved north");
            }
        } else if(y >= Z_MAP_TILES_H) {
            if(g_game.universeY < Z_UNIVERSE_DIM - 1) {
                g_game.universeY++;
                g_game.moveAction = Z_SCREEN_MOVE_DOWN;
                g_game.lastPlayerX = x;
                z_game_setLogAction("Moved south");
            }
        } else {
            ZCompMap* map = a_entity_requireComponent(g_game.map, "map");

            if(z_comp_map_getTileEntity(map, x, y) == NULL) {
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
        }

        if(g_game.moveAction != Z_SCREEN_MOVE_NONE) {
            a_state_pop();
            a_state_push("playGame");
        }
    }

    if(a_button_getOnce(z_controls.main)) {
        if(moodType == Z_MOOD_GOOD) {
            moodType = Z_MOOD_EVIL;
        } else if(moodType == Z_MOOD_EVIL) {
            moodType = Z_MOOD_GOOD;
        }

        z_comp_mood_setType(mood, moodType);
        g_game.lastPlayerMood = moodType;
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
        g_game.oldMapScreen = a_sprite_blank(Z_MAP_PIXEL_W,
                                             Z_MAP_PIXEL_H,
                                             false);
        g_game.newMapScreen = a_sprite_blank(Z_MAP_PIXEL_W,
                                             Z_MAP_PIXEL_H,
                                             false);
        g_game.lines = a_list_new();
        g_game.lastPlayerMood = Z_MOOD_GOOD;
    }

    A_STATE_BODY
    {
        a_state_push("playGame");
    }

    A_STATE_FREE
    {
        A_LIST_ITERATE(g_game.lines, char*, line) {
            free(line);
        }

        a_list_free(g_game.lines);
    }
}

A_STATE(playGame)
{
    A_STATE_INIT
    {
        a_component_declare("damage", z_comp_damage_size(), NULL);
        a_component_declare("health", z_comp_health_size(), NULL);
        a_component_declare("hud", 0, NULL);
        a_component_declare("input", z_comp_input_size(), NULL);
        a_component_declare("interact", z_comp_interact_size(), z_comp_interact_free);
        a_component_declare("map", z_comp_map_size(), NULL);
        a_component_declare("mood", z_comp_mood_size(), NULL);
        a_component_declare("position", z_comp_position_size(), NULL);
        a_component_declare("sprite", z_comp_sprite_size(), NULL);

        a_system_declare("drawHud", "hud", z_system_hudDraw, NULL, false);
        a_system_declare("drawMap", "map", z_system_mapDraw, NULL, false);
        a_system_declare("drawSprites", "position sprite", z_system_sprite, NULL, false);
        a_system_declare("getInputs", "input", z_system_input, NULL, false);
        a_system_declare("runInteractions", "interact", z_system_interact, NULL, false);

        a_system_tick("getInputs runInteractions");
        a_system_draw("drawMap drawSprites drawHud");

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

        g_game.playerShip = a_entity_new();
        a_entity_setId(g_game.playerShip, "playerShip");
        z_comp_map_setTileEntity(map, x, y, g_game.playerShip);
        z_comp_position_init(a_entity_addComponent(g_game.playerShip, "position"), x, y);
        z_comp_sprite_init(a_entity_addComponent(g_game.playerShip, "sprite"), "playerShip");
        z_comp_input_init(a_entity_addComponent(g_game.playerShip, "input"), playerInput);
        z_comp_health_init(a_entity_addComponent(g_game.playerShip, "health"), 100);
        z_comp_damage_init(a_entity_addComponent(g_game.playerShip, "damage"), 4);
        z_comp_mood_init(a_entity_addComponent(g_game.playerShip, "mood"), g_game.lastPlayerMood);
        a_entity_addComponent(g_game.playerShip, "hud");

        for(int i = 1 + a_random_int(10); i--; ) {
            AEntity* sat = a_entity_new();
            ZCompSprite* sprite = a_entity_addComponent(sat, "sprite");
            ZCompPosition* position = a_entity_addComponent(sat, "position");
            ZCompHealth* health = a_entity_addComponent(sat, "health");
            ZCompInteract* interact = a_entity_addComponent(sat, "interact");

            do {
                x = 1 + a_random_int(Z_MAP_TILES_W - 2);
                y = 1 + a_random_int(Z_MAP_TILES_H - 2);
            } while(z_comp_map_getTileEntity(map, x, y) != NULL);

            z_comp_map_setTileEntity(map, x, y, sat);
            z_comp_position_init(position, x, y);
            z_comp_sprite_init(sprite, "satellite");
            z_comp_health_init(health, 15);
            z_comp_interact_init(interact, "Satellite");
        }

        if(g_game.moveAction != Z_SCREEN_MOVE_NONE) {
            // Draw the new map and feed it to the transition state
            a_state_push("nextScreen");

            a_screen_setTargetSprite(g_game.newMapScreen);

            a_system_flushNewEntities();
            a_entity_mute(g_game.playerShip);
            a_system_execute("drawMap drawSprites");
            a_entity_unmute(g_game.playerShip);

            a_screen_resetTarget();
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
        int xInc = 0, yInc = 0;
        int xStart = 0, yStart = 0;
        int numMoves = 0;
        int movesPerFrame = Z_MAP_TILE_DIM / 4;

        switch(g_game.moveAction) {
            case Z_SCREEN_MOVE_LEFT: {
                xInc = 1;
                xStart = -Z_MAP_PIXEL_W;
                numMoves = Z_MAP_PIXEL_W;
            } break;

            case Z_SCREEN_MOVE_RIGHT: {
                xInc = -1;
                xStart = Z_MAP_PIXEL_W;
                numMoves = Z_MAP_PIXEL_W;
            } break;

            case Z_SCREEN_MOVE_UP: {
                yInc = 1;
                yStart = -Z_MAP_PIXEL_H;
                numMoves = Z_MAP_PIXEL_H;
            } break;

            case Z_SCREEN_MOVE_DOWN: {
                yInc = -1;
                yStart = Z_MAP_PIXEL_H;
                numMoves = Z_MAP_PIXEL_H;
            } break;

            default: break;
        }

        a_screen_setClip(0, 0, Z_MAP_PIXEL_W, Z_MAP_PIXEL_H);

        A_STATE_LOOP
        {
            a_sprite_blit(g_game.newMapScreen, xStart, yStart);
            a_sprite_blit(g_game.oldMapScreen,
                          xStart + xInc * Z_MAP_PIXEL_W,
                          yStart + yInc * Z_MAP_PIXEL_H);

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
