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
#include "util_log.h"

#include "component_cargo.h"
#include "component_map.h"
#include "component_position.h"
#include "component_trade.h"

#include "entity_asteroid.h"
#include "entity_player.h"
#include "entity_ship.h"

#include "state_game.h"

#define Z_UNIVERSE_DIM 256

typedef struct ZGameScreen {
    AEntity* map;
    AList* entities; // list of AEntity, map is included in it
    ASprite* mapScreen; // used for scrolling between screens
} ZGameScreen;

typedef struct ZGame {
    unsigned initialSeed;
    unsigned universeX, universeY;
    ZScreenMove moveAction;
    AEntity* player;
    ZLog* log;
    ZGameScreen current;
    ZGameScreen staging;
    bool allowPlayer;
    bool allowAi;
    ATimer* aiTurnDelay;
} ZGame;

static ZGame g_game;

static void z_game_initScreen(ZGameScreen* Screen)
{
    Screen->map = NULL;
    Screen->entities = a_list_new();
    Screen->mapScreen = a_sprite_newBlank(Z_MAP_PIXEL_W, Z_MAP_PIXEL_H, false);
}

static void z_game_freeScreen(ZGameScreen* Screen)
{
    a_list_free(Screen->entities);
    a_sprite_free(Screen->mapScreen);
}

static void z_game_createStagingScreen(void)
{
    // This screen's unique seed
    a_random_setSeed(g_game.initialSeed
                     + g_game.universeY * Z_UNIVERSE_DIM
                     + g_game.universeX);

    g_game.staging.map = a_entity_new("map");
    a_list_addLast(g_game.staging.entities, g_game.staging.map);

    ZCompMap* map = a_entity_addComponent(g_game.staging.map, "map");
    z_comp_map_init(map);

    // Set player's new position
    int x, y;
    ZCompPosition* playerPosition = a_entity_requireComponent(g_game.player,
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
    z_comp_map_setTileEntity(map, x, y, g_game.player);

    // Asteroids for mining
    if(a_random_getInt(2) == 0) {
        for(int i = 1 + a_random_getInt(4); i--; ) {
            AEntity* e = z_entity_asteroid_new(map);
            a_list_addLast(g_game.staging.entities, e);
        }
    }

    // Satellites
    if(a_random_getInt(2) == 0) {
        for(int i = 1 + a_random_getInt(4); i--; ) {
            AEntity* e = z_entity_ship_satellite(map);
            a_list_addLast(g_game.staging.entities, e);
        }
    }

    // Weak space ships
    if(a_random_getInt(2) == 0) {
        for(int i = a_random_getInt(8); i--; ) {
            AEntity* e = z_entity_ship_neutralShip(map);
            a_list_addLast(g_game.staging.entities, e);
        }
    }

    // Stronger space ships
    if(a_random_getInt(4) == 0) {
        for(int i = 1 + a_random_getInt(5); i--; ) {
            AEntity* e = z_entity_ship_patrolShip(map);
            a_list_addLast(g_game.staging.entities, e);
        }
    }

    // Strong ships that actively pursue player
    if(a_random_getInt(8) == 0) {
        for(int i = 1 + a_random_getInt(3); i--; ) {
            AEntity* e = z_entity_ship_fighterShip(map);
            a_list_addLast(g_game.staging.entities, e);
        }
    }

    // Trade ships
    if(a_random_getInt(6) == 0) {
        for(int i = 1 + a_random_getInt(2); i--; ) {
            AEntity* e = z_entity_ship_merchantShip(map);
            a_list_addLast(g_game.staging.entities, e);
        }
    }
}

static void z_game_prepareNextScreen(void)
{
    // Capture the old map, including the player ship
    a_screen_targetPushSprite(g_game.current.mapScreen);
    a_system_execute("drawMap drawSprites");
    a_screen_targetPop();

    // Mute current map entities and the map itself
    A_LIST_ITERATE(g_game.current.entities, AEntity*, e) {
        a_entity_mute(e);
    }

    // Create a new screen and set new player position
    z_game_createStagingScreen();
    a_system_flushNewEntities();

    // Capture the new screen except for the HUD and player ship
    a_screen_targetPushSprite(g_game.staging.mapScreen);

    // Player ship gets unmuted in z_game_flipStagingScreen in the new screen
    a_entity_mute(g_game.player);
    a_system_execute("drawMap drawSprites");

    a_screen_targetPop();

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
    a_entity_unmute(g_game.player);

    A_LIST_ITERATE(g_game.current.entities, AEntity*, e) {
        a_entity_unmute(e);
    }

    g_game.moveAction = Z_SCREEN_MOVE_NONE;
    g_game.allowPlayer = true;
    g_game.allowAi = false;
    a_timer_stop(g_game.aiTurnDelay);

    z_game_tradeOff(NULL);
}

bool z_game_moveScreen(ZScreenMove Direction)
{
    bool ret = false;

    switch(Direction) {
        case Z_SCREEN_MOVE_LEFT: {
            if(g_game.universeX > 0) {
                g_game.universeX--;
                g_game.moveAction = Direction;
                ret = true;
            }
        } break;

        case Z_SCREEN_MOVE_RIGHT: {
            if(g_game.universeX < Z_UNIVERSE_DIM - 1) {
                g_game.universeX++;
                g_game.moveAction = Direction;
                ret = true;
            }
        } break;

        case Z_SCREEN_MOVE_UP: {
            if(g_game.universeY > 0) {
                g_game.universeY--;
                g_game.moveAction = Direction;
                ret = true;
            }
        } break;

        case Z_SCREEN_MOVE_DOWN: {
            if(g_game.universeY < Z_UNIVERSE_DIM - 1) {
                g_game.universeY++;
                g_game.moveAction = Direction;
                ret = true;
            }
        } break;

        default: break;
    }

    if(g_game.moveAction != Z_SCREEN_MOVE_NONE) {
        z_game_prepareNextScreen();
    }

    return ret;
}

AEntity* z_game_getPlayer(void)
{
    return g_game.player;
}

AEntity* z_game_getMap(void)
{
    return g_game.current.map;
}

void z_game_getUniverseCoords(unsigned* X, unsigned* Y)
{
    *X = g_game.universeX;
    *Y = g_game.universeY;
}

bool z_game_isPlayerTurn(void)
{
    return g_game.allowPlayer;
}

bool z_game_isAiTurn(void)
{
    return g_game.allowAi;
}

void z_game_playerActed(void)
{
    // Player did something, it'll be AI's turn when the timer expires
    g_game.allowPlayer = false;
    a_timer_start(g_game.aiTurnDelay);
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
    a_list_removeItem(g_game.current.entities, Entity);
}

void z_game_log(const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    z_log_log(g_game.log, Format, args);

    va_end(args);
}

AList* z_game_getLogLines(void)
{
    return z_log_getLines(g_game.log);
}

void z_game_tradeOn(AEntity* Merchant)
{
    ZCompTrade* trade = a_entity_requireComponent(Merchant, "trade");
    z_comp_trade_setOn(trade, true);

    a_button_release(z_controls.up);
    a_button_release(z_controls.down);

    a_system_mute("playerInput ai");
    a_system_unmute("tradeTick tradeDraw");
}

void z_game_tradeOff(AEntity* Merchant)
{
    if(Merchant) {
        ZCompTrade* trade = a_entity_requireComponent(Merchant, "trade");
        z_comp_trade_setOn(trade, false);
    }

    a_system_mute("tradeTick tradeDraw");
    a_system_unmute("playerInput ai");
}

A_STATE(playGame)
{
    A_STATE_INIT
    {
        g_game.initialSeed = a_random_getSeed();
        g_game.universeX = Z_UNIVERSE_DIM / 2;
        g_game.universeY = Z_UNIVERSE_DIM / 2;
        g_game.moveAction = Z_SCREEN_MOVE_NONE;
        g_game.player = z_entity_player_new();
        g_game.log = z_log_new(6);
        g_game.aiTurnDelay = a_timer_new(A_TIMER_FRAMES, a_fps_msToFrames(250));

        z_game_initScreen(&g_game.current);
        z_game_initScreen(&g_game.staging);
        z_game_createStagingScreen();

        a_system_tick("playerInput ai tradeTick");
        a_system_draw("drawMap drawSprites drawHud tradeDraw");
    }

    A_STATE_BODY
    {
        z_game_flipStagingScreen();

        A_STATE_LOOP {
            // This runs before the systems. Establish whether
            // the Player or the AI can do anything this frame.

            if(a_timer_isExpired(g_game.aiTurnDelay)) {
                // AI can go now, player is already off if timer was running
                g_game.allowAi = true;
                a_timer_stop(g_game.aiTurnDelay);
            } else {
                if(g_game.allowAi) {
                    // AI just went the last frame, player can go now
                    g_game.allowPlayer = true;
                }

                // Timer is either pending or not running, so AI can't go
                g_game.allowAi = false;
            }

            if(a_entity_isRemoved(g_game.player)) {
                a_state_pop();
            }
        }
    }

    A_STATE_FREE
    {
        z_log_free(g_game.log);
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

        a_screen_clipSet(0, 0, Z_MAP_PIXEL_W, Z_MAP_PIXEL_H);

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

        a_screen_clipReset();
    }
}
