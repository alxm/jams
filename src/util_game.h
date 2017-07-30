/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Despot 3900 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Despot 3900 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Despot 3900.  If not, see <http://www.gnu.org/licenses/>.
*/

extern ZGame* z_game_init(void);
extern void z_game_free(ZGame* Game);

extern int z_game_getTimeInMonths(const ZGame* Game);
extern unsigned z_game_getNumImprisoned(const ZGame* Game);
extern void z_game_setNumImprisoned(ZGame* Game, unsigned Num);
extern ZDespot* z_game_getDespot(const ZGame* Game);

extern void z_game_log(const ZGame* Game, AFont* Font, const char* Format, ...);
extern void z_game_logInc(const ZGame* Game);
extern void z_game_logDec(const ZGame* Game);
extern void z_game_logTick(const ZGame* Game);

extern void z_game_staveOffRevolt(ZGame* Game);
extern void z_game_staveOffCoup(ZGame* Game);

extern void z_game_turn(ZGame* Game);
extern void z_game_handleMenu(ZGame* Game);

extern void z_game_draw(const ZGame* Game);
extern void z_game_drawMenu(const ZGame* Game);
