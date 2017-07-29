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

#define Z_YEARS_TO_MONTHS(Years)  ((Years) * 12)
#define Z_MONTHS_TO_YEARS(Months) ((Months) / 12)

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
    Game->timeInMonths = Z_YEARS_TO_MONTHS(3900);
    Game->despot.dobInMonths = Game->timeInMonths - Z_YEARS_TO_MONTHS(30);
    Game->despot.health = 80;
    Game->despot.wealth = 1000;
    Game->despot.popularity = 50;
    Game->despot.loyalty = 50;
    Game->log = z_log_new(8);
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
                game_log(&game, NULL, "New turn");
            }

            z_log_tick(game.log);

            A_STATE_LOOP_DRAW
            {
                a_pixel_setHex(0xaaff88);
                a_draw_fill();

                z_log_draw(game.log, 0, 0);
            }
        }
    }

    A_STATE_FREE
    {
        game_free(&game);
    }
}
