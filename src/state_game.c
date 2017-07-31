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
#include "util_sfx.h"
#include "util_time.h"

#define Z_MONTHS_PER_TURN 6

ZGame* z_game;

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

        if(health <= 0) {
            z_game_log(Game, Z_LOG_BAD, "Despot died");
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
    int wealth = z_despot_getWealth(despot);

    unsigned revoltCounter = z_game_getRevoltCounter(Game);

    if(revoltCounter > 0) {
        if(revoltCounter == Z_REVOLT_COUNT_MAX) {
            z_game_log(Game,
                       Z_LOG_BAD,
                       "The peasants revolt against the Despot");

            int chance = 1;
            int total = 1;
            chance += age >= 60; total++;
            chance += age >= 70; total++;
            chance += age >= 80; total++;
            chance += health < 50; total++;
            chance += health < 40; total++;
            chance += health < 30; total++;
            chance += health < 20; total++;
            chance += health < 10; total++;
            chance += popularity < 50; total++;
            chance += popularity < 40; total++;
            chance += popularity < 30; total++;
            chance += popularity < 20; total++;
            chance += popularity < 10; total++;
            chance += loyalty < 50; total++;

            z_game_logInc(Game);

            z_game_log(Game,
                       Z_LOG_NEUTRAL,
                       "Peasants have a %d%% probability of success",
                       100 * chance / total);

            if(a_random_chance(chance, total)) {
                z_game_log(Game,
                           Z_LOG_BAD,
                           "The revolt was successful, Despot is history");
                z_despot_setHealth(despot, 0);
            } else {
                z_game_log(Game,
                           Z_LOG_GOOD,
                           "The Despot squashed the peasants' rebellion");

                z_game_logInc(Game);
                z_despot_setWealth(despot, wealth + 1000);
                z_game_log(Game, Z_LOG_GOOD, "GLORY TO DESPOT!");
                z_game_logDec(Game);
            }

            z_game_logDec(Game);

            z_game_setRevoltCounter(Game, 0);
            z_game_setCoupCounter(Game, 0);

            return false;
        } else {
            z_game_setRevoltCounter(Game, ++revoltCounter);

            z_game_log(Game,
                       Z_LOG_BAD,
                       "%d%% of the way to a peasant's revolt",
                       100 * revoltCounter / Z_REVOLT_COUNT_MAX);
        }
    } else if(popularity < Z_REVOLT_THRESHOLD) {
        z_game_setRevoltCounter(Game, 1);

        z_game_log(Game,
                   Z_LOG_BAD,
                   "Despot's popularity amongst peasants is below %d%%",
                   Z_REVOLT_THRESHOLD);

        z_game_logInc(Game);
        z_game_log(Game, Z_LOG_BAD, "Revolt is imminent");
        z_game_logDec(Game);
    }

    return true;
}

static bool game_coup(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);
    int popularity = z_despot_getPopularity(despot);
    int loyalty = z_despot_getLoyalty(despot);
    int health = z_despot_getHealth(despot);
    int age = z_despot_getAgeInYears(despot);
    int wealth = z_despot_getWealth(despot);

    unsigned revoltCounter = z_game_getRevoltCounter(Game);
    unsigned coupCounter = z_game_getCoupCounter(Game);

    if(coupCounter > 0) {
        if(coupCounter == Z_COUP_COUNT_MAX) {
            z_game_log(Game,
                       Z_LOG_BAD,
                       "The nobles stage a coup against the Despot");

            int chance = 1;
            int total = 1;
            chance += age >= 60; total++;
            chance += age >= 70; total++;
            chance += age >= 80; total++;
            chance += health < 50; total++;
            chance += health < 40; total++;
            chance += health < 30; total++;
            chance += health < 20; total++;
            chance += health < 10; total++;
            chance += popularity < 50; total++;
            chance += loyalty < 50; total++;
            chance += loyalty < 40; total++;
            chance += loyalty < 30; total++;
            chance += loyalty < 20; total++;
            chance += loyalty < 10; total++;

            z_game_logInc(Game);

            z_game_log(Game,
                       Z_LOG_NEUTRAL,
                       "Nobles have a %d%% probability of success",
                       100 * chance / total);

            if(a_random_chance(chance, total)) {
                z_game_log(Game,
                           Z_LOG_BAD,
                           "The coup was successful, Despot is history");
                z_despot_setHealth(despot, 0);
            } else {
                z_game_log(Game,
                           Z_LOG_GOOD,
                           "The Despot squashed the nobles' coup");

                z_game_logInc(Game);
                z_despot_setWealth(despot, wealth + 2000);
                z_game_log(Game, Z_LOG_GOOD, "GLORY TO DESPOT!");
                z_game_logDec(Game);
            }

            z_game_logDec(Game);

            z_game_setRevoltCounter(Game, revoltCounter / 2);
            z_game_setCoupCounter(Game, 0);

            return false;
        } else {
            z_game_setCoupCounter(Game, ++coupCounter);

            z_game_log(Game,
                       Z_LOG_BAD,
                       "%d%% of the way to a noble's coup",
                       100 * coupCounter / Z_COUP_COUNT_MAX);
        }
    } else if(z_despot_getLoyalty(despot) < Z_COUP_THRESHOLD) {
        z_game_setCoupCounter(Game, 1);

        z_game_log(Game,
                   Z_LOG_BAD,
                   "Nobles' loyalty to the Despot is below %d%%",
                   Z_COUP_THRESHOLD);

        z_game_logInc(Game);
        z_game_log(Game, Z_LOG_BAD, "A coup is imminent");
        z_game_logDec(Game);
    }

    return true;
}

static bool game_turn(ZGame* Game)
{
    int timeInMonths = z_game_getTimeInMonths(Game) + Z_MONTHS_PER_TURN;
    z_game_setTimeInMonths(Game, timeInMonths);

    a_sfx_play(z_sfx.newTurn);

    if(z_time_monthsIntoYear(timeInMonths) == 0) {
        z_game_log(Game, Z_LOG_NEUTRAL, "A year passed - GLORY TO THE DESPOT!");
        z_game_setNumImprisoned(Game, 0);
        z_game_setNumWars(Game, 0);
    } else {
        z_game_log(Game,
                   Z_LOG_NEUTRAL,
                   "Another %d months passed",
                   Z_MONTHS_PER_TURN);
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

        z_game_log(z_game,
                   Z_LOG_NEUTRAL,
                   "Welcome to %s",
                   a_settings_getString("app.title"));
        z_game_log(z_game, Z_LOG_NEUTRAL, "");
        z_game_log(z_game,
                   Z_LOG_NEUTRAL,
                   "You inherited your seat on the throne as is custom.");
        z_game_log(z_game,
                   Z_LOG_NEUTRAL,
                   "You get to affect change on your country every %d months.",
                   Z_MONTHS_PER_TURN);
        z_game_log(z_game,
                   Z_LOG_NEUTRAL,
                   "See how rich you can get before you kick the bucket.");
        z_game_log(z_game, Z_LOG_NEUTRAL, "");

        a_state_push("flushLog");
    }

    A_STATE_BODY
    {
        ZDespot* despot = z_game_getDespot(z_game);

        z_controls_release();

        A_STATE_LOOP
        {
            if(z_despot_getHealth(despot) <= 0
                || z_despot_getWealth(despot) <= 0) {

                a_state_push("gameOver");
            } else {
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
            }

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

A_STATE(gameOver)
{
    A_STATE_INIT
    {
        z_game_log(z_game, Z_LOG_BAD, "GAME OVER");
        a_state_push("flushLog");
    }

    A_STATE_BODY
    {
        z_controls_release();
        z_game_setInstructions(z_game, "Press SPACE BAR to start a new game");

        A_STATE_LOOP
        {
            if(a_button_getPressedOnce(z_controls.action)) {
                a_state_pop();
                a_state_pop();
                a_state_push("game");
            }

            A_STATE_LOOP_DRAW
            {
                z_game_draw(z_game);
            }
        }
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
