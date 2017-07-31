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

typedef enum {
    Z_MENU_MAIN,
    Z_MENU_TAX,
    Z_MENU_GIVE,
    Z_MENU_IMPRISON,
    Z_MENU_NUM
} ZMenu;

typedef enum {
    Z_LOG_GOOD,
    Z_LOG_BAD,
} ZLogHint;

#define Z_REVOLT_THRESHOLD 50
#define Z_REVOLT_COUNT_MAX 10

#define Z_COUP_THRESHOLD   50
#define Z_COUP_COUNT_MAX   10

extern ZGame* z_game_init(void);
extern void z_game_free(ZGame* Game);

extern void z_game_setInstructions(ZGame* Game, const char* Text);

extern int z_game_getTimeInMonths(const ZGame* Game);
extern void z_game_setTimeInMonths(ZGame* Game, int Value);

extern unsigned z_game_getNumImprisoned(const ZGame* Game);
extern void z_game_setNumImprisoned(ZGame* Game, unsigned Num);

extern unsigned z_game_getNumWars(const ZGame* Game);
extern void z_game_setNumWars(ZGame* Game, unsigned Num);

extern unsigned z_game_getRevoltCounter(const ZGame* Game);
extern void z_game_setRevoltCounter(ZGame* Game, unsigned Value);

extern unsigned z_game_getCoupCounter(const ZGame* Game);
extern void z_game_setCoupCounter(ZGame* Game, unsigned Value);

extern ZDespot* z_game_getDespot(const ZGame* Game);

extern void z_game_log(const ZGame* Game, ZLogHint Hint, const char* Format, ...);
extern void z_game_logInc(const ZGame* Game);
extern void z_game_logDec(const ZGame* Game);
extern bool z_game_logTick(const ZGame* Game);

extern bool z_game_handleMenu(ZGame* Game);

extern void z_game_draw(const ZGame* Game);
extern void z_game_drawMenu(const ZGame* Game);

extern AMenu* z_game_getMenu(const ZGame* Game);
extern void z_game_setMenu(ZGame* Game, ZMenu Menu);
extern bool z_game_runMenuHandler(ZGame* Game, ZMenuItem* Item);
