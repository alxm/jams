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
#include "util_time.h"

ZGame* z_game;

static void game_checkGameOver(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    if(z_despot_getHealth(despot) <= 0 || z_despot_getWealth(despot) <= 0) {
        a_state_pop();
    }
}

static bool game_health(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    int age = z_despot_getAgeInYears(despot);
    int health = z_despot_getHealth(despot);
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
        z_despot_setHealth(despot, health);

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
    ZDespot* despot = z_game_getDespot(Game);
    int popularity = z_despot_getPopularity(despot);
    int loyalty = z_despot_getLoyalty(despot);
    int health = z_despot_getHealth(despot);
    int age = z_despot_getAgeInYears(despot);

    unsigned revoltCounter = z_game_getRevoltCounter(Game);

    if(revoltCounter > 0) {
        revoltCounter++;
        z_game_setRevoltCounter(Game, revoltCounter);

        if(revoltCounter >= Z_REVOLT_COUNT_MAX) {
            z_game_log(Game,
                       NULL,
                       "The peasants revolt against the Despot",
                       Z_REVOLT_COUNT_MAX);

            int chance = 1;
            chance += age >= 60;
            chance += age >= 70;
            chance += age >= 80;
            chance += health < 50;
            chance += health < 40;
            chance += health < 30;
            chance += health < 20;
            chance += health < 10;
            chance += popularity < 50;
            chance += popularity < 40;
            chance += popularity < 30;
            chance += popularity < 20;
            chance += popularity < 10;
            chance += loyalty < 50;

            z_game_logInc(Game);

            z_game_log(Game,
                       NULL,
                       "Peasants have a %d%% probability of success",
                       100 * chance / 15);

            if(a_random_chance(chance, 15)) {
                z_game_log(Game,
                           NULL,
                           "The revolt was successful, Despot is history",
                           Z_REVOLT_COUNT_MAX);
                z_despot_setHealth(despot, 0);
            } else {
                z_game_log(Game,
                           NULL,
                           "The Despot squashed the peasants' rebellion",
                           Z_REVOLT_COUNT_MAX);

                z_game_logInc(Game);
                z_game_log(Game,
                           NULL,
                           "GLORY TO DESPOT!",
                           Z_REVOLT_COUNT_MAX);
                z_game_logDec(Game);
            }

            z_game_logDec(Game);

            z_game_setRevoltCounter(Game, 0);
            z_game_setCoupCounter(Game, 0);

            return false;
        }
    } else if(popularity < Z_REVOLT_THRESHOLD) {
        z_game_setRevoltCounter(Game, 1);

        z_game_log(Game,
                   NULL,
                   "Despot's popularity amongst peasants is below %d%%",
                   Z_REVOLT_THRESHOLD);

        z_game_logInc(Game);
        z_game_log(Game, NULL, "Revolt is imminent");
        z_game_logDec(Game);
    }

    return true;
}

static bool game_coup(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);
    unsigned revoltCounter = z_game_getRevoltCounter(Game);
    unsigned coupCounter = z_game_getCoupCounter(Game);

    if(coupCounter > 0) {
        coupCounter++;
        z_game_setCoupCounter(Game, coupCounter);

        if(coupCounter >= Z_COUP_COUNT_MAX) {
            z_game_log(Game,
                       NULL,
                       "Coup counter reached %d",
                       Z_COUP_COUNT_MAX);

            // Stage coup

            z_game_setRevoltCounter(Game, revoltCounter / 2);
            z_game_setCoupCounter(Game, 0);

            return false;
        }
    } else if(z_despot_getLoyalty(despot) < Z_COUP_THRESHOLD) {
        z_game_setCoupCounter(Game, 1);

        z_game_log(Game,
                   NULL,
                   "Nobles' loyalty to the Despot is below %d%%",
                   Z_COUP_THRESHOLD);

        z_game_logInc(Game);
        z_game_log(Game, NULL, "A coup is imminent");
        z_game_logDec(Game);
    }

    return true;
}

static bool game_turn(ZGame* Game)
{
    int timeInMonths = z_game_getTimeInMonths(Game) + 1;
    z_game_setTimeInMonths(Game, timeInMonths);

    if(z_time_monthsIntoYear(timeInMonths) == 0) {
        z_game_log(Game, NULL, "A year passed - GLORY TO THE DESPOT!");
        z_game_setNumImprisoned(Game, 0);
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

A_STATE(game)
{
    A_STATE_INIT
    {
        z_game = z_game_init();
        z_game_log(z_game, NULL, "Hello, world");

        a_state_push("flushLog");
    }

    A_STATE_BODY
    {
        z_controls_release();

        A_STATE_LOOP
        {
            z_game_logTick(z_game);

            if(a_button_getPressedOnce(z_controls.action)) {
                if(game_turn(z_game)) {
                    a_state_push("actionMenu");
                }

                a_state_push("flushLog");
            } else {
                z_game_setInstructions(z_game,
                                       "Press SPACE BAR for next turn");
            }

            game_checkGameOver(z_game);

            A_STATE_LOOP_DRAW
            {
                z_game_draw(z_game);
            }
        }
    }

    A_STATE_FREE
    {
        z_game_free(z_game);
    }
}

A_STATE(flushLog)
{
    A_STATE_BODY
    {
        z_controls_release();

        A_STATE_LOOP
        {
            if(z_game_logTick(z_game)) {
                z_game_setInstructions(z_game, "Wait...");
            } else {
                a_state_pop();
            }

            A_STATE_LOOP_DRAW
            {
                z_game_draw(z_game);
            }
        }
    }
}
