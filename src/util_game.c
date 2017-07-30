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

#include <a2x.h>

#include "util_types.h"

#include "util_controls.h"
#include "util_despot.h"
#include "util_game.h"
#include "util_log.h"
#include "util_time.h"

#include "state_action.h"

#define Z_REVOLT_THRESHOLD 50
#define Z_REVOLT_COUNT_MAX 10

#define Z_COUP_THRESHOLD   50
#define Z_COUP_COUNT_MAX   10

struct ZGame {
    const char* instructions;
    int timeInMonths;
    unsigned revoltCounter;
    unsigned coupCounter;
    unsigned numImprisonedThisYear;
    ZDespot* despot;
    ZLog* log;
    AMenu* menus[Z_MENU_NUM];
    ZMenu currentMenu;
};

typedef void ZMenuHandler(ZGame* Game);

typedef struct ZMenuItem {
    const char* title;
    const char* blurb;
    ZMenuHandler* handler;
} ZMenuItem;

static ZMenuItem* menu_item_new(const char* Title, const char* Blurb, ZMenuHandler Handler)
{
    ZMenuItem* item = a_mem_malloc(sizeof(ZMenuItem));

    item->title = Title;
    item->blurb = Blurb;
    item->handler = Handler;

    return item;
}

static void menu_item_free(ZMenuItem* Item)
{
    free(Item);
}

ZGame* z_game_init(void)
{
    ZGame* g = a_mem_malloc(sizeof(ZGame));

    g->instructions = "";
    g->timeInMonths = z_time_yearsToMonths(3900);
    g->revoltCounter = 0;
    g->coupCounter = 0;
    g->numImprisonedThisYear = 0;

    g->despot = z_despot_new(g,
                             g->timeInMonths - z_time_yearsToMonths(30),
                             80,
                             1000,
                             50,
                             50);

    g->log = z_log_new(10);

    for(ZMenu s = Z_MENU_NUM; s--; ) {
        g->menus[s] = a_menu_new(z_controls.down,
                                z_controls.up,
                                z_controls.action,
                                NULL);
    }

    a_menu_addItem(g->menus[Z_MENU_MAIN],
                   menu_item_new("Do Nothing",
                                 "Relax to improve your health!",
                                 z_action_doNothing));

    a_menu_addItem(g->menus[Z_MENU_MAIN],
                   menu_item_new("Collect Taxes",
                                 "Get some more money!",
                                 z_action_collectTaxes));
    {
        a_menu_addItem(g->menus[Z_MENU_TAX],
                       menu_item_new("Tax the peasants",
                                     "There's a lot of them to tax!",
                                     z_action_collectTaxesFromPeasants));

        a_menu_addItem(g->menus[Z_MENU_TAX],
                       menu_item_new("Tax the nobles",
                                     "Despot giveth and Despot taketh away!",
                                     z_action_collectTaxesFromNobles));
    }

    a_menu_addItem(g->menus[Z_MENU_MAIN],
                   menu_item_new("Give Money",
                                 "Spread the wealth!",
                                 z_action_giveMoney));
    {
        a_menu_addItem(g->menus[Z_MENU_GIVE],
                       menu_item_new("Give to the peasants",
                                     "Throw some crumbs!",
                                     z_action_giveMoneyToPeasants));

        a_menu_addItem(g->menus[Z_MENU_GIVE],
                       menu_item_new("Give to the nobles",
                                     "The rich get richer!",
                                     z_action_giveMoneyToNobles));
    }

    a_menu_addItem(g->menus[Z_MENU_MAIN],
                   menu_item_new("Imprison Opponents",
                                 "How dare they question you!",
                                 z_action_imprison));
    {
        a_menu_addItem(g->menus[Z_MENU_IMPRISON],
                       menu_item_new("Imprison rebellious peasants",
                                     "That'll show them!",
                                     z_action_imprisonPeasants));

        a_menu_addItem(g->menus[Z_MENU_IMPRISON],
                       menu_item_new("Imprison a corrupt noble",
                                     "You made them, you break them!",
                                     z_action_imprisonNobles));
    }

    a_menu_addItem(g->menus[Z_MENU_MAIN],
                   menu_item_new("Wage War",
                                 "Always a coin toss!",
                                 z_action_wageWar));

    g->currentMenu = Z_MENU_MAIN;

    return g;
}

void z_game_free(ZGame* Game)
{
    for(ZMenu s = Z_MENU_NUM; s--; ) {
        a_menu_freeEx(Game->menus[s], (AListFree*)menu_item_free);
    }

    z_log_free(Game->log);
    free(Game);
}

void z_game_setInstructions(ZGame* Game, const char* Text)
{
    Game->instructions = Text;
}

int z_game_getTimeInMonths(const ZGame* Game)
{
    return Game->timeInMonths;
}

unsigned z_game_getNumImprisoned(const ZGame* Game)
{
    return Game->numImprisonedThisYear;
}

void z_game_setNumImprisoned(ZGame* Game, unsigned Num)
{
    Game->numImprisonedThisYear = Num;
}

ZDespot* z_game_getDespot(const ZGame* Game)
{
    return Game->despot;
}

void z_game_log(const ZGame* Game, AFont* Font, const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    z_log_log(Game->log, Font, Format, args);

    va_end(args);
}

void z_game_logInc(const ZGame* Game)
{
    z_log_inc(Game->log);
}

void z_game_logDec(const ZGame* Game)
{
    z_log_dec(Game->log);
}

bool z_game_logTick(const ZGame* Game)
{
    return z_log_tick(Game->log);
}

void z_game_staveOffRevolt(ZGame* Game)
{
    Game->revoltCounter = a_math_minu(Game->revoltCounter, 1);

    if(Game->revoltCounter > 0) {
        z_game_log(Game,
                   NULL,
                   "Staved off revolt for another %d months",
                   Z_REVOLT_COUNT_MAX - 1);
    } else {
        z_game_log(Game,
                   NULL,
                   "A needless effort");
    }
}

void z_game_staveOffCoup(ZGame* Game)
{
    Game->coupCounter = a_math_minu(Game->coupCounter, 1);

    if(Game->coupCounter > 0) {
        z_game_log(Game,
                   NULL,
                   "Staved off coup for another %d months",
                   Z_COUP_COUNT_MAX - 1);
    } else {
        z_game_log(Game,
                   NULL,
                   "A needless effort");
    }
}

static bool game_health(ZGame* Game)
{
    int age = z_despot_getAgeInYears(Game->despot);

    int health = z_despot_getHealth(Game->despot);
    int healthDec = 0;

    if(age >= 90) {
        healthDec = 5;
    } if(age >= 80) {
        healthDec = 4;
    } else if(age >= 70) {
        healthDec = 3;
    } else if(age >= 60) {
        healthDec = 2;
    }

    if(healthDec > 0) {
        health -= healthDec;
        z_despot_setHealth(Game->despot, health);

        z_game_log(Game, NULL, "Despot lost %d health", healthDec);

        if(health <= 0) {
            z_game_log(Game, NULL, "Despot died");
            return false;
        }
    }

    return true;
}

static bool game_revolt(ZGame* Game)
{
    if(Game->revoltCounter > 0) {
        if(Game->revoltCounter++ >= Z_REVOLT_COUNT_MAX) {
            z_game_log(Game,
                       NULL,
                       "Revolt counter reached %d",
                       Z_REVOLT_COUNT_MAX);

            // Stage revolt

            Game->revoltCounter = 0;
            Game->coupCounter = 0;

            return false;
        }
    } else if(z_despot_getPopularity(Game->despot) < Z_REVOLT_THRESHOLD) {
        Game->revoltCounter = 1;

        z_game_log(Game,
                   NULL,
                   "Despot's popularity slipped below %d%%",
                   Z_REVOLT_THRESHOLD);
    }

    return true;
}

static bool game_coup(ZGame* Game)
{
    if(Game->coupCounter > 0) {
        if(Game->coupCounter++ >= Z_COUP_COUNT_MAX) {
            z_game_log(Game,
                       NULL,
                       "Coup counter reached %d",
                       Z_COUP_COUNT_MAX);

            // Stage coup

            Game->revoltCounter /= 2;
            Game->coupCounter = 0;

            return false;
        }
    } else if(z_despot_getLoyalty(Game->despot) < Z_COUP_THRESHOLD) {
        Game->coupCounter = 1;

        z_game_log(Game,
                   NULL,
                   "Nobles' loyalty to the Despot slipped below %d%%",
                   Z_COUP_THRESHOLD);
    }

    return true;
}

bool z_game_turn(ZGame* Game)
{
    Game->timeInMonths++;

    if(z_time_monthsIntoYear(Game->timeInMonths) == 0) {
        z_game_log(Game, NULL, "A year passed");

        Game->numImprisonedThisYear = 0;
    } else {
        z_game_log(Game, NULL, "Another month passed");
    }

    z_game_logInc(Game);

    if(game_health(Game) && game_revolt(Game) && game_coup(Game)) {
        return true;
    }

    z_game_logDec(Game);

    return false;
}

bool z_game_handleMenu(ZGame* Game)
{
    AMenu* menu = Game->menus[Game->currentMenu];

    a_menu_handleInput(menu);

    if(a_menu_getState(menu) == A_MENU_STATE_SELECTED) {
        ZMenuItem* selected = a_menu_getSelectedItem(menu);
        selected->handler(Game);

        a_menu_reset(menu);
        return true;
    }

    return false;
}

static void game_drawStats(const ZGame* Game)
{
    int startX = a_screen_getWidth() / 2;
    int startY = 0;
    int width = a_screen_getWidth() - startX;
    int height = a_screen_getHeight() / 2;

    a_pixel_setHex(0x88ffaa);
    a_draw_rectangle(startX, startY, width, height);

    a_font_setCoords(startX + 2, startY + 2);

    a_font_printf("YEAR %d", z_time_monthsToYears(Game->timeInMonths));
    a_font_newLine();

    a_font_printf("AGE %d", z_despot_getAgeInYears(Game->despot));
    a_font_newLine();

    a_font_printf("HEALTH %d/100", z_despot_getHealth(Game->despot));
    a_font_newLine();

    a_font_printf("WEALTH %d", z_despot_getWealth(Game->despot));
    a_font_newLine();

    a_font_printf("POPULARITY %d/100", z_despot_getPopularity(Game->despot));
    a_font_newLine();

    a_font_printf("LOYALTY %d/100", z_despot_getLoyalty(Game->despot));
    a_font_newLine();

    a_font_newLine();

    a_font_printf("REVOLT IN %d/%d", Game->revoltCounter, Z_REVOLT_COUNT_MAX);
    a_font_newLine();

    a_font_printf("COUP IN %d/%d", Game->coupCounter, Z_COUP_COUNT_MAX);
    a_font_newLine();
}

static void game_drawLog(const ZGame* Game)
{
    int startX = 0;
    int startY = a_screen_getHeight() / 2;
    int width = a_screen_getWidth();
    int height = a_screen_getHeight() / 2 - 27;

    a_pixel_setHex(0xffff88);
    a_draw_rectangle(startX, startY, width, height);

    z_log_draw(Game->log, startX + 2, startY + 2);
}

static void game_drawHelp(const ZGame* Game)
{
    int startX = 0;
    int startY = a_screen_getHeight() - 27;
    int width = a_screen_getWidth();
    int height = 27;

    a_pixel_setHex(0xaaffff);
    a_draw_rectangle(startX, startY, width, height);

    a_font_setCoords(startX + 2, startY + 10);
    a_font_print(Game->instructions);
}

void z_game_draw(const ZGame* Game)
{
    a_pixel_setHex(0xaaff88);
    a_draw_fill();

    game_drawStats(Game);
    game_drawLog(Game);
    game_drawHelp(Game);
}

void z_game_drawMenu(const ZGame* Game)
{
    AMenu* menu = Game->menus[Game->currentMenu];

    a_font_setCoords(2, 2);

    A_LIST_ITERATE(a_menu_getItems(menu), ZMenuItem*, item) {
        if(a_menu_isItemSelected(menu, item)) {
            a_font_print("> ");
        }

        a_font_print(item->title);
        a_font_newLine();
    }

    ZMenuItem* selected = a_menu_getSelectedItem(menu);
    a_font_newLine();
    a_font_print(selected->blurb);
}
