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

typedef enum {
    Z_SCREEN_MOVE_NONE,
    Z_SCREEN_MOVE_LEFT,
    Z_SCREEN_MOVE_RIGHT,
    Z_SCREEN_MOVE_UP,
    Z_SCREEN_MOVE_DOWN
} ZScreenMove;

extern bool z_game_moveScreen(ZScreenMove Direction);

extern AEntity* z_game_getPlayer(void);
extern AEntity* z_game_getMap(void);

extern void z_game_getUniverseCoords(unsigned* X, unsigned* Y);

extern bool z_game_isPlayerTurn(void);
extern bool z_game_isAiTurn(void);
extern void z_game_playerActed(void);

extern void z_game_removeEntity(AEntity* Entity);

extern void z_game_log(const char* Format, ...);
extern AList* z_game_getLogLines(void);

extern A_STATE(playGame);
extern A_STATE(nextScreen);
