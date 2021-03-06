/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef struct ZStateGame ZStateGame;

extern A_STATE(game);
extern A_STATE(hypnotize);

extern void z_state_game_getOrigin(const ZStateGame* Game, int* X, int* Y);
extern AEntity* z_state_game_getLog(const ZStateGame* Game);
extern AEntity* z_state_game_getMap(const ZStateGame* Game);
extern AEntity* z_state_game_getPlayer(const ZStateGame* Game);
extern AEntity* z_state_game_getAlarm(const ZStateGame* Game);
extern AColMap* z_state_game_getVolumeColMap(const ZStateGame* Game);

extern void z_state_game_gameOver(ZStateGame* Game);
extern void z_state_game_hypnotize(ZStateGame* Game);
