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

#include "component_ai.h"
#include "component_cargo.h"
#include "component_damage.h"
#include "component_health.h"
#include "component_input.h"
#include "component_interact.h"
#include "component_map.h"
#include "component_mood.h"
#include "component_position.h"
#include "component_sprite.h"

#define Z_UNIVERSE_DIM 256

typedef enum {
    Z_SCREEN_MOVE_NONE,
    Z_SCREEN_MOVE_LEFT,
    Z_SCREEN_MOVE_RIGHT,
    Z_SCREEN_MOVE_UP,
    Z_SCREEN_MOVE_DOWN
} ZScreenMove;

typedef struct ZGameScreen {
    AEntity* map;
    AList* entities; // list of AEntity, map is included in it
    ASprite* mapScreen; // used for scrolling between screens
} ZGameScreen;

typedef struct ZGame {
    unsigned initialSeed;
    unsigned universeX, universeY;
    ZScreenMove moveAction;
    AEntity* playerShip;
    AList* lines;
    ZGameScreen current;
    ZGameScreen staging;
    bool waitForPlayerTurn;
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

            if(a_list_size(g_game.lines) > 6) {
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

        ZCompMap* map = a_entity_requireComponent(g_game.current.map, "map");
        z_comp_map_setTileEntity(map, x, y, NULL);
    }

    a_entity_mute(Entity);
    a_entity_remove(Entity);
    a_list_remove(g_game.current.entities, Entity);
}

bool z_game_waitingForPlayer(void)
{
    return g_game.waitForPlayerTurn;
}

static void z_game_initScreen(ZGameScreen* Screen)
{
    Screen->map = NULL;
    Screen->entities = a_list_new();
    Screen->mapScreen = a_sprite_blank(Z_MAP_PIXEL_W, Z_MAP_PIXEL_H, false);
}

static void z_game_freeScreen(ZGameScreen* Screen)
{
    a_list_free(Screen->entities);
    a_sprite_free(Screen->mapScreen);
}

static AEntity* spawnEntity(ZCompMap* Map, const char* Sprite, const char* Name)
{
    AEntity* e = a_entity_new();
    a_entity_setId(e, Name);

    a_list_addLast(g_game.staging.entities, e);

    int x, y;
    ZCompPosition* position = a_entity_addComponent(e, "position");

    do {
        x = 1 + a_random_int(Z_MAP_TILES_W - 2);
        y = 1 + a_random_int(Z_MAP_TILES_H - 2);
    } while(z_comp_map_getTileEntity(Map, x, y) != NULL);

    z_comp_position_init(position, x, y);
    z_comp_map_setTileEntity(Map, x, y, e);

    ZCompSprite* sprite = a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite, Sprite);

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

typedef struct ZAiContextShip {
    int x;
} ZAiContextShip;

static void shipAi(AEntity* Entity, ZAiMessageType Type, AEntity* Relevant)
{
    ZCompAi* ai = a_entity_requireComponent(Entity, "ai");
    ZAiContextShip* context = z_comp_ai_getContext(ai);

    switch(Type) {
        case Z_AI_MESSAGE_GREETED: {
            z_game_setLogAction("Hello to you too, %s",
                                a_entity_getId(Relevant));
        } break;

        case Z_AI_MESSAGE_ATTACKED: {
            z_game_setLogAction("Ouch, %s... %d",
                                a_entity_getId(Relevant),
                                ++context->x);
        } break;
    }
}

static void addShipAi(AEntity* Entity)
{
    ZCompAi* ai = a_entity_addComponent(Entity, "ai");
    z_comp_ai_init(ai, shipAi, sizeof(ZAiContextShip));
}

static void z_game_createStagingScreen(void)
{
    // This screen's unique seed
    a_random_setSeed(g_game.initialSeed
                     + g_game.universeY * Z_UNIVERSE_DIM
                     + g_game.universeX);

    g_game.staging.map = a_entity_new();
    a_list_addLast(g_game.staging.entities, g_game.staging.map);

    ZCompMap* map = a_entity_addComponent(g_game.staging.map, "map");
    z_comp_map_init(map);

    // Set player's new position
    int x, y;
    ZCompPosition* playerPosition = a_entity_requireComponent(g_game.playerShip,
                                                              "position");
    z_comp_position_getCoords(playerPosition, &x, &y);

    switch(g_game.moveAction) {
        case Z_SCREEN_MOVE_LEFT: {
            x = Z_MAP_TILES_W - 1;
        } break;

        case Z_SCREEN_MOVE_RIGHT: {
            x = 0;
        } break;

        case Z_SCREEN_MOVE_UP: {
            y = Z_MAP_TILES_H - 1;
        } break;

        case Z_SCREEN_MOVE_DOWN: {
            y = 0;
        } break;

        default: break;
    }

    z_comp_position_setCoords(playerPosition, x, y);
    z_comp_map_setTileEntity(map, x, y, g_game.playerShip);

    // Satellites
    for(int i = 1 + a_random_int(4); i--; ) {
        AEntity* e = spawnEntity(map, "satellite1", "Satellite");
        addHealth(e, 15);
        addCargo(e, Z_CARGO_TYPE_CREDS, a_random_int(3));
    }

    // Space ships
    for(int i = 1 + a_random_int(4); i--; ) {
        AEntity* e = spawnEntity(map, "ship1", "Space Ship");
        addHealth(e, 20);
        addCargo(e, Z_CARGO_TYPE_CREDS, a_random_int(5));
        addCargo(e, Z_CARGO_TYPE_FUEL, a_random_int(3));
        addShipAi(e);
    }
}

static void z_game_prepareNextScreen(void)
{
    // Capture the old map, including the player ship
    a_screen_setTargetSprite(g_game.current.mapScreen);
    a_system_execute("drawMap drawSprites");
    a_screen_resetTarget();

    // Mute current map entities and the map itself
    A_LIST_ITERATE(g_game.current.entities, AEntity*, e) {
        a_entity_mute(e);
    }

    // Create a new screen and set new player position
    z_game_createStagingScreen();
    a_system_flushNewEntities();

    // Capture the new screen except for the HUD and player ship
    a_screen_setTargetSprite(g_game.staging.mapScreen);

    a_entity_mute(g_game.playerShip);
    a_system_execute("drawMap drawSprites");

    a_screen_resetTarget();

    // Mute new entities so they don't flash on screen before scroll
    A_LIST_ITERATE(g_game.staging.entities, AEntity*, e) {
        a_entity_mute(e);
    }

    // Unmute old entities so there isn't an empty flash before scroll
    A_LIST_ITERATE(g_game.current.entities, AEntity*, e) {
        a_entity_unmute(e);
    }

    // Scroll to new screen
    a_state_push("nextScreen");
}

static void z_game_flipStagingScreen(void)
{
    ZGameScreen current = g_game.current;

    // Mute and remove old entities
    A_LIST_ITERATE(current.entities, AEntity*, e) {
        a_entity_mute(e);
        a_entity_remove(e);
    }

    current.map = NULL;
    a_list_clear(current.entities);

    g_game.current = g_game.staging;
    g_game.staging = current;

    // Unmute new entities
    a_entity_unmute(g_game.playerShip);

    A_LIST_ITERATE(g_game.current.entities, AEntity*, e) {
        a_entity_unmute(e);
    }

    g_game.moveAction = Z_SCREEN_MOVE_NONE;
    g_game.waitForPlayerTurn = true;
}

static void playerInput(AEntity* Entity)
{
    bool acted = false;

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
                z_game_setLogAction("Moved west");
            }
        } else if(x >= Z_MAP_TILES_W) {
            if(g_game.universeX < Z_UNIVERSE_DIM - 1) {
                g_game.universeX++;
                g_game.moveAction = Z_SCREEN_MOVE_RIGHT;
                z_game_setLogAction("Moved east");
            }
        } else if(y < 0) {
            if(g_game.universeY > 0) {
                g_game.universeY--;
                g_game.moveAction = Z_SCREEN_MOVE_UP;
                z_game_setLogAction("Moved north");
            }
        } else if(y >= Z_MAP_TILES_H) {
            if(g_game.universeY < Z_UNIVERSE_DIM - 1) {
                g_game.universeY++;
                g_game.moveAction = Z_SCREEN_MOVE_DOWN;
                z_game_setLogAction("Moved south");
            }
        } else {
            ZCompMap* map = a_entity_requireComponent(g_game.current.map, "map");

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

        if(g_game.moveAction != Z_SCREEN_MOVE_NONE) {
            z_game_prepareNextScreen();
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

    g_game.waitForPlayerTurn = !acted;
}

A_STATE(playGame)
{
    A_STATE_INIT
    {
        g_game.initialSeed = a_random_getSeed();
        g_game.universeX = Z_UNIVERSE_DIM / 2;
        g_game.universeY = Z_UNIVERSE_DIM / 2;
        g_game.moveAction = Z_SCREEN_MOVE_NONE;
        g_game.playerShip = NULL;
        g_game.lines = a_list_new();
        z_game_initScreen(&g_game.current);
        z_game_initScreen(&g_game.staging);

        g_game.playerShip = a_entity_new();
        a_entity_setId(g_game.playerShip, "playerShip");
        z_comp_position_init(a_entity_addComponent(g_game.playerShip, "position"), Z_MAP_TILES_W / 2, Z_MAP_TILES_H / 2);
        z_comp_sprite_init(a_entity_addComponent(g_game.playerShip, "sprite"), "playerShip");
        z_comp_input_init(a_entity_addComponent(g_game.playerShip, "input"), playerInput);
        z_comp_health_init(a_entity_addComponent(g_game.playerShip, "health"), 100);
        z_comp_damage_init(a_entity_addComponent(g_game.playerShip, "damage"), 4);
        z_comp_mood_init(a_entity_addComponent(g_game.playerShip, "mood"), Z_MOOD_GOOD);
        z_comp_cargo_init(a_entity_addComponent(g_game.playerShip, "cargo"));
        a_entity_addComponent(g_game.playerShip, "hud");

        z_game_createStagingScreen();

        // Player does something, process that, AI does something, process that
        a_system_tick("getInputs runInteractions ai runInteractions");
        a_system_draw("drawMap drawSprites drawHud");
    }

    A_STATE_BODY
    {
        z_game_flipStagingScreen();

        A_STATE_LOOP;
    }

    A_STATE_FREE
    {
        A_LIST_ITERATE(g_game.lines, char*, line) {
            free(line);
        }

        a_list_free(g_game.lines);

        z_game_freeScreen(&g_game.current);
        z_game_freeScreen(&g_game.staging);
    }
}

A_STATE(nextScreen)
{
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
            a_sprite_blit(g_game.staging.mapScreen, xStart, yStart);
            a_sprite_blit(g_game.current.mapScreen,
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
}
