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
#include "util_gfx.h"
#include "util_log.h"
#include "util_sfx.h"
#include "util_strings.h"
#include "util_time.h"

#include "state_action.h"

struct ZGame {
    const char* instructions;
    int timeInMonths;
    unsigned revoltCounter;
    unsigned coupCounter;
    unsigned numImprisonedThisYear;
    unsigned numWarsThisYear;
    ZDespot* despot;
    ZLog* log;
    AMenu* menus[Z_MENU_NUM];
    ZMenu currentMenu;
};

typedef bool ZMenuHandler(ZGame* Game);

struct ZMenuItem {
    const char* title;
    const char* blurb;
    ZMenuHandler* handler;
};

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

static bool menu_back(ZGame* Game)
{
    z_game_setMenu(Game, Z_MENU_MAIN);

    return false;
}

ZGame* z_game_init(void)
{
    ZGame* g = a_mem_malloc(sizeof(ZGame));

    g->instructions = "";
    g->timeInMonths = z_time_yearsToMonths(3900);
    g->revoltCounter = 0;
    g->coupCounter = 0;
    g->numImprisonedThisYear = 0;
    g->numWarsThisYear = 0;

    g->despot = z_despot_new(g,
                             g->timeInMonths - z_time_yearsToMonths(50),
                             80,
                             10000,
                             75,
                             75);

    g->log = z_log_new(10);

    for(ZMenu s = Z_MENU_NUM; s--; ) {
        g->menus[s] = a_menu_new(z_controls.down,
                                 z_controls.up,
                                 z_controls.action,
                                 NULL);

        a_menu_addSounds(g->menus[s], z_sfx.menuSelect, NULL, z_sfx.menuBrowse);
    }

    a_menu_addItem(g->menus[Z_MENU_MAIN],
                   menu_item_new(Z_STR_MENU_NOTHING_TITLE,
                                 Z_STR_MENU_NOTHING_BLURB,
                                 z_action_doNothing));

    a_menu_addItem(g->menus[Z_MENU_MAIN],
                   menu_item_new(Z_STR_MENU_TAX_TITLE,
                                 Z_STR_MENU_TAX_BLURB,
                                 z_action_collectTaxes));
    {
        a_menu_addItem(g->menus[Z_MENU_TAX],
                       menu_item_new(Z_STR_MENU_TAX_P_TITLE,
                                     Z_STR_MENU_TAX_P_BLURB,
                                     z_action_collectTaxesFromPeasants));

        a_menu_addItem(g->menus[Z_MENU_TAX],
                       menu_item_new(Z_STR_MENU_TAX_N_TITLE,
                                     Z_STR_MENU_TAX_N_BLURB,
                                     z_action_collectTaxesFromNobles));

        a_menu_addItem(g->menus[Z_MENU_TAX],
                       menu_item_new(Z_STR_MENU_BACK_TITLE,
                                     Z_STR_MENU_BACK_BLURB,
                                     menu_back));
    }

    a_menu_addItem(g->menus[Z_MENU_MAIN],
                   menu_item_new(Z_STR_MENU_GIVE_TITLE,
                                 Z_STR_MENU_GIVE_BLURB,
                                 z_action_giveMoney));
    {
        a_menu_addItem(g->menus[Z_MENU_GIVE],
                       menu_item_new(Z_STR_MENU_GIVE_P_TITLE,
                                     Z_STR_MENU_GIVE_P_BLURB,
                                     z_action_giveMoneyToPeasants));

        a_menu_addItem(g->menus[Z_MENU_GIVE],
                       menu_item_new(Z_STR_MENU_GIVE_N_TTLE,
                                     Z_STR_MENU_GIVE_N_BLURB,
                                     z_action_giveMoneyToNobles));

        a_menu_addItem(g->menus[Z_MENU_GIVE],
                       menu_item_new(Z_STR_MENU_BACK_TITLE,
                                     Z_STR_MENU_BACK_BLURB,
                                     menu_back));
    }

    a_menu_addItem(g->menus[Z_MENU_MAIN],
                   menu_item_new(Z_STR_MENU_IMP_TITLE,
                                 Z_STR_MENU_IMP_BLURB,
                                 z_action_imprison));
    {
        a_menu_addItem(g->menus[Z_MENU_IMPRISON],
                       menu_item_new(Z_STR_MENU_IMP_P_TITLE,
                                     Z_STR_MENU_IMP_P_BLURB,
                                     z_action_imprisonPeasants));

        a_menu_addItem(g->menus[Z_MENU_IMPRISON],
                       menu_item_new(Z_STR_MENU_IMP_N_TITLE,
                                     Z_STR_MENU_IMP_N_BLURB,
                                     z_action_imprisonNobles));

        a_menu_addItem(g->menus[Z_MENU_IMPRISON],
                       menu_item_new(Z_STR_MENU_BACK_TITLE,
                                     Z_STR_MENU_BACK_BLURB,
                                     menu_back));
    }

    a_menu_addItem(g->menus[Z_MENU_MAIN],
                   menu_item_new(Z_STR_MENU_WAR_TITLE,
                                 Z_STR_MENU_WAR_BLURB,
                                 z_action_wageWar));

    g->currentMenu = Z_MENU_MAIN;

    return g;
}

void z_game_free(ZGame* Game)
{
    for(ZMenu s = Z_MENU_NUM; s--; ) {
        a_menu_freeEx(Game->menus[s], (AListFree*)menu_item_free);
    }

    z_despot_free(Game->despot);
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

void z_game_setTimeInMonths(ZGame* Game, int Value)
{
    Game->timeInMonths = Value;
}

unsigned z_game_getNumImprisoned(const ZGame* Game)
{
    return Game->numImprisonedThisYear;
}

void z_game_setNumImprisoned(ZGame* Game, unsigned Num)
{
    Game->numImprisonedThisYear = Num;
}

unsigned z_game_getNumWars(const ZGame* Game)
{
    return Game->numWarsThisYear;
}

void z_game_setNumWars(ZGame* Game, unsigned Num)
{
    Game->numWarsThisYear = Num;
}

unsigned z_game_getRevoltCounter(const ZGame* Game)
{
    return Game->revoltCounter;
}

void z_game_setRevoltCounter(ZGame* Game, unsigned Value)
{
    Game->revoltCounter = a_math_minu(Value, Z_REVOLT_COUNT_MAX);
}

unsigned z_game_getCoupCounter(const ZGame* Game)
{
    return Game->coupCounter;
}

void z_game_setCoupCounter(ZGame* Game, unsigned Value)
{
    Game->coupCounter = a_math_minu(Value, Z_COUP_COUNT_MAX);
}

ZDespot* z_game_getDespot(const ZGame* Game)
{
    return Game->despot;
}

void z_game_log(const ZGame* Game, ZLogHint Hint, const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    AFont* font = z_fonts.grayLight;

    if(Hint == Z_LOG_GOOD) {
        a_sfx_play(z_sfx.outcomeGood);
    } else if(Hint == Z_LOG_BAD) {
        a_sfx_play(z_sfx.outcomeBad);
    }

    z_log_log(Game->log, font, Format, args);

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

static void game_drawStats(const ZGame* Game)
{
    int startX = 3 * a_screen_getWidth() / 4;
    int startY = 0;
    int width = a_screen_getWidth() - startX;
    int height = a_screen_getHeight() / 2;

    a_pixel_setPixel(z_colors.greenMedium);
    a_draw_rectangle(startX, startY, width, height);

    a_font_setFont(z_fonts.grayDark);
    a_font_setCoords(startX + 2, startY + 2);

    a_font_printf("YEAR %d, month %d",
                  z_time_monthsToYears(Game->timeInMonths),
                  z_time_monthsIntoYear(Game->timeInMonths) + 1);
    a_font_newLine();

    a_font_printf("AGE %d", z_despot_getAgeInYears(Game->despot));
    a_font_newLine();

    a_font_printf("HEALTH %d%%", z_despot_getHealth(Game->despot));
    a_font_newLine();

    a_font_printf("WEALTH %d", z_despot_getWealth(Game->despot));
    a_font_newLine();

    a_font_printf("POPULARITY %d%%", z_despot_getPopularity(Game->despot));
    a_font_newLine();

    a_font_printf("LOYALTY %d%%", z_despot_getLoyalty(Game->despot));
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
    int startY = a_screen_getHeight() / 2 + 27;
    int width = a_screen_getWidth();
    int height = a_screen_getHeight() / 2 - 27;

    a_pixel_setPixel(z_colors.grayDark);
    a_draw_rectangle(startX, startY, width, height);

    z_log_draw(Game->log, startX + 2, startY + 2);
}

static void game_drawHelp(const ZGame* Game)
{
    int startX = 0;
    int startY = a_screen_getHeight() / 2;
    int width = a_screen_getWidth();
    int height = 27;

    a_pixel_setPixel(z_colors.greenLight);
    a_draw_rectangle(startX, startY, width, height);

    a_font_setFont(z_fonts.grayDark);
    a_font_setCoords(startX + 2, startY + 10);
    a_font_print(Game->instructions);
}

void z_game_draw(const ZGame* Game)
{
    a_pixel_setPixel(z_colors.grayDark);
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
            a_font_setFont(z_fonts.redLight);
            a_font_print("> ");
        }

        a_font_setFont(z_fonts.grayLight);
        a_font_print(item->title);
        a_font_newLine();
    }

    ZMenuItem* selected = a_menu_getSelectedItem(menu);
    a_font_newLine();
    a_font_print(selected->blurb);
}

AMenu* z_game_getMenu(const ZGame* Game)
{
    return Game->menus[Game->currentMenu];
}

void z_game_setMenu(ZGame* Game, ZMenu Menu)
{
    Game->currentMenu = Menu;
}

bool z_game_runMenuHandler(ZGame* Game, ZMenuItem* Item)
{
    return Item->handler(Game);
}
