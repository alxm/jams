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

#include "state_game.h"

#include "util_controls.h"
#include "util_despot.h"
#include "util_log.h"
#include "util_time.h"

#define Z_REVOLT_THRESHOLD 50
#define Z_REVOLT_COUNT_MAX 10

#define Z_COUP_THRESHOLD   50
#define Z_COUP_COUNT_MAX   10

struct ZGame {
    int timeInMonths;
    unsigned revoltCounter;
    unsigned coupCounter;
    ZDespot* despot;
    ZLog* log;
    AMenu* actionMenu;
};

typedef void ZMenuHandler(ZGame* Game);

typedef struct ZMenuItem {
    const char* title;
    const char* blurb;
    ZMenuHandler* handler;
} ZMenuItem;

static ZGame g_game;

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

static void menu_handler_doNothing(ZGame* Game)
{
    z_game_log(Game,
               NULL,
               "Despot got some well-earned rest");

    ZDespot* despot = Game->despot;

    z_despot_setHealth(despot, z_despot_getHealth(despot) + 1);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 1);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 1);
}

static void menu_handler_collectTaxes(ZGame* Game)
{
    ZDespot* despot = Game->despot;

    if(a_random_chance(1, 2)) {
        z_game_log(Game,
                   NULL,
                   "Despot taxed the peasants");

        z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 1);
    } else {
        z_game_log(Game,
                   NULL,
                   "Despot taxed the nobles");

        z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 1);
    }

    z_despot_setWealth(despot, z_despot_getWealth(despot) + 100);
}

static void menu_handler_giveMoney(ZGame* Game)
{
    ZDespot* despot = Game->despot;

    if(a_random_chance(1, 2)) {
        z_game_log(Game,
                   NULL,
                   "Despot was generous to the peasants");

        z_despot_setPopularity(despot, z_despot_getPopularity(despot) + 1);
        z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 1);
    } else {
        z_game_log(Game,
                   NULL,
                   "Despot was generous to the nobles");

        z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 1);
        z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) + 1);
    }

    z_despot_setWealth(despot, z_despot_getWealth(despot) - 100);
}

static void menu_handler_imprisonOpponents(ZGame* Game)
{
    A_UNUSED(Game);
    printf("menu_handler_imprisonOpponents\n");
}

static void menu_handler_wageWar(ZGame* Game)
{
    A_UNUSED(Game);
    printf("menu_handler_wageWar\n");
}

static void game_init(ZGame* Game)
{
    Game->timeInMonths = z_time_yearsToMonths(3900);
    Game->revoltCounter = 0;
    Game->coupCounter = 0;

    Game->despot = z_despot_new(Game,
                                Game->timeInMonths - z_time_yearsToMonths(30),
                                80,
                                1000,
                                50,
                                50);

    Game->log = z_log_new(12);

    Game->actionMenu = a_menu_new(z_controls.down,
                                  z_controls.up,
                                  z_controls.action,
                                  NULL);

    a_menu_addItem(Game->actionMenu,
                   menu_item_new("Do Nothing",
                                 "Relax to improve your health!",
                                 menu_handler_doNothing));

    a_menu_addItem(Game->actionMenu,
                   menu_item_new("Collect Taxes",
                                 "Get some more money!",
                                 menu_handler_collectTaxes));

    a_menu_addItem(Game->actionMenu,
                   menu_item_new("Give Money",
                                 "Spread the wealth!",
                                 menu_handler_giveMoney));

    a_menu_addItem(Game->actionMenu,
                   menu_item_new("Imprison Opponents",
                                 "How dare they question you!",
                                 menu_handler_imprisonOpponents));

    a_menu_addItem(Game->actionMenu,
                   menu_item_new("Wage War",
                                 "Always a coin toss!",
                                 menu_handler_wageWar));
}

static void game_free(ZGame* Game)
{
    z_log_free(Game->log);

    A_LIST_ITERATE(a_menu_getItems(Game->actionMenu), ZMenuItem*, item) {
        menu_item_free(item);
    }

    a_menu_free(Game->actionMenu);
}

static void game_checkGameOver(ZGame* Game)
{
    if(z_despot_getHealth(Game->despot) <= 0
        || z_despot_getWealth(Game->despot) <= 0) {

        a_state_pop();
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

static void game_turn(ZGame* Game)
{
    Game->timeInMonths++;

    z_game_log(Game,
             NULL,
             "A month passed. %d months into the year.",
             z_time_monthsIntoYear(Game->timeInMonths));

    if(game_health(Game) && game_revolt(Game) && game_coup(Game)) {
        a_state_push("actionMenu");
    }
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
    int height = a_screen_getHeight() / 2;

    a_pixel_setHex(0xffff88);
    a_draw_rectangle(startX, startY, width, height);

    z_log_draw(Game->log, startX + 2, startY + 6);
}

static void game_draw(const ZGame* Game)
{
    a_pixel_setHex(0xaaff88);
    a_draw_fill();

    game_drawStats(Game);
    game_drawLog(Game);
}

A_STATE(game)
{
    A_STATE_INIT
    {
        game_init(&g_game);
        z_game_log(&g_game, NULL, "Hello, world");
    }

    A_STATE_BODY
    {
        z_controls_release();

        A_STATE_LOOP
        {
            if(a_button_getPressedOnce(z_controls.action)) {
                game_turn(&g_game);
            }

            z_log_tick(g_game.log);
            game_checkGameOver(&g_game);

            A_STATE_LOOP_DRAW
            {
                game_draw(&g_game);
            }
        }
    }

    A_STATE_FREE
    {
        game_free(&g_game);
    }
}

A_STATE(actionMenu)
{
    A_STATE_BODY
    {
        A_STATE_LOOP
        {
            AMenu* menu = g_game.actionMenu;

            a_menu_handleInput(menu);

            if(a_menu_getState(menu) == A_MENU_STATE_SELECTED) {
                ZMenuItem* selected = a_menu_getSelectedItem(menu);
                selected->handler(&g_game);

                a_menu_reset(menu);
                a_state_pop();
            }

            A_STATE_LOOP_DRAW
            {
                game_draw(&g_game);

                a_font_setCoords(0, 0);

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
        }
    }
}

int z_game_getTimeInMonths(const ZGame* Game)
{
    return Game->timeInMonths;
}

void z_game_log(ZGame* Game, AFont* Font, const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    z_log_log(Game->log, Font, Format, args);

    va_end(args);
}
