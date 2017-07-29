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

#include "util_controls.h"
#include "util_log.h"

static inline int time_yearsToMonths(int Years)
{
    return Years * 12;
}

static inline int time_monthsToYears(int Months)
{
    return Months / 12;
}

static inline int time_monthsIntoYear(int Months)
{
    return Months % 12;
}

typedef struct ZDespot {
    int dobInMonths;
    int health;
    int wealth;
    int popularity;
    int loyalty;
} ZDespot;

typedef struct ZGame {
    int timeInMonths;
    ZDespot despot;
    ZLog* log;
} ZGame;

static void game_init(ZGame* Game)
{
    Game->timeInMonths = time_yearsToMonths(3900);
    Game->despot.dobInMonths = Game->timeInMonths - time_yearsToMonths(30);
    Game->despot.health = 80;
    Game->despot.wealth = 1000;
    Game->despot.popularity = 50;
    Game->despot.loyalty = 50;
    Game->log = z_log_new(12);
}

static void game_free(ZGame* Game)
{
    z_log_free(Game->log);
}

static void game_log(ZGame* Game, AFont* Font, const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    z_log_log(Game->log, Font, Format, args);

    va_end(args);
}

static void game_newTurn(ZGame* Game)
{
    Game->timeInMonths++;

    game_log(Game,
             NULL,
             "A month passed. %d months into the year.",
             time_monthsIntoYear(Game->timeInMonths));

    int age = time_monthsToYears(Game->timeInMonths - Game->despot.dobInMonths);
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

    Game->despot.health -= healthDec;

    if(healthDec > 0) {
        if(Game->despot.health <= 0) {
            game_log(Game, NULL, "Despot died");
        } else {
            game_log(Game, NULL, "Despot lost %d health", healthDec);
        }
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

    a_font_printf("YEAR %d", time_monthsToYears(Game->timeInMonths));
    a_font_newLine();

    a_font_printf("AGE %d",
                  time_monthsToYears(
                    Game->timeInMonths - Game->despot.dobInMonths));
    a_font_newLine();

    a_font_printf("HEALTH %d/100", Game->despot.health);
    a_font_newLine();

    a_font_printf("WEALTH %d", Game->despot.wealth);
    a_font_newLine();

    a_font_printf("POPULARITY %d/100", Game->despot.popularity);
    a_font_newLine();

    a_font_printf("LOYALTY %d/100", Game->despot.loyalty);
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

A_STATE(game)
{
    static ZGame game;

    A_STATE_INIT
    {
        game_init(&game);
        game_log(&game, NULL, "Hello, world");
    }

    A_STATE_BODY
    {
        z_controls_release();

        A_STATE_LOOP
        {
            if(a_button_getPressedOnce(z_controls.action)) {
                game_newTurn(&game);
            }

            z_log_tick(game.log);

            A_STATE_LOOP_DRAW
            {
                a_pixel_setHex(0xaaff88);
                a_draw_fill();

                game_drawStats(&game);
                game_drawLog(&game);
            }
        }
    }

    A_STATE_FREE
    {
        game_free(&game);
    }
}
