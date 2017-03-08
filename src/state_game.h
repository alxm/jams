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

extern A_STATE(newGame);
extern A_STATE(playGame);
extern A_STATE(nextScreen);

extern void z_game_getUniverseCoords(unsigned* X, unsigned* Y);
extern AEntity* z_game_getPlayer(void);

extern void z_game_setLogAction(const char* Format, ...);
extern AList* z_game_getLogLines(void);
