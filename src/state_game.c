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
#include "util_strings.h"
#include "util_time.h"

#define Z_MONTHS_PER_TURN 6

ZGame* z_game;

static bool game_health(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    int age = z_despot_getAgeInYears(despot);
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

    int newHealth = z_despot_getHealth(despot) - healthDec;
    z_despot_setHealth(despot, newHealth);

    return newHealth > 0;
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
            z_game_log(Game, Z_LOG_BAD, Z_STR_REVOLT);

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
                       Z_STR_REVOLT_PROB,
                       100 * chance / total);

            if(a_random_chance(chance, total)) {
                z_game_log(Game, Z_LOG_BAD, Z_STR_REVOLT_SUCCESS);
                z_despot_setHealth(despot, 0);
            } else {
                z_game_log(Game, Z_LOG_GOOD, Z_STR_REVOLT_FAIL);

                z_game_logInc(Game);
                z_despot_setWealth(despot, wealth + 1000);
                z_game_log(Game, Z_LOG_GOOD, Z_STR_DESPOT_GLORY);
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
                       Z_STR_REVOLT_PROGRESS,
                       100 * revoltCounter / Z_REVOLT_COUNT_MAX);
        }
    } else if(popularity < Z_REVOLT_THRESHOLD) {
        z_game_setRevoltCounter(Game, 1);

        z_game_log(Game, Z_LOG_BAD, Z_STR_REVOLT_THRESHOLD, Z_REVOLT_THRESHOLD);
        z_game_logInc(Game);
        z_game_log(Game, Z_LOG_BAD, Z_STR_REVOLT_IMMINENT);
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
            z_game_log(Game, Z_LOG_BAD, Z_STR_COUP);

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
                       Z_STR_COUP_PROB,
                       100 * chance / total);

            if(a_random_chance(chance, total)) {
                z_game_log(Game, Z_LOG_BAD, Z_STR_COUP_SUCCESS);
                z_despot_setHealth(despot, 0);
            } else {
                z_game_log(Game, Z_LOG_GOOD, Z_STR_COUP_FAIL);

                z_game_logInc(Game);
                z_despot_setWealth(despot, wealth + 2000);
                z_game_log(Game, Z_LOG_GOOD, Z_STR_DESPOT_GLORY);
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
                       Z_STR_COUP_PROGRESS,
                       100 * coupCounter / Z_COUP_COUNT_MAX);
        }
    } else if(z_despot_getLoyalty(despot) < Z_COUP_THRESHOLD) {
        z_game_setCoupCounter(Game, 1);

        z_game_log(Game, Z_LOG_BAD, Z_STR_COUP_THRESHOLD, Z_COUP_THRESHOLD);
        z_game_logInc(Game);
        z_game_log(Game, Z_LOG_BAD, Z_STR_COUP_IMMINENT);
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
        z_game_log(Game, Z_LOG_NEUTRAL, Z_STR_TURN_YEAR);
        z_game_setNumImprisoned(Game, 0);
        z_game_setNumWars(Game, 0);
    } else {
        z_game_log(Game, Z_LOG_NEUTRAL, Z_STR_TURN_MONTHS, Z_MONTHS_PER_TURN);
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
                   Z_STR_INTRO_1,
                   a_settings_getString("app.title"));
        z_game_log(z_game, Z_LOG_NEUTRAL, "");
        z_game_log(z_game, Z_LOG_NEUTRAL, Z_STR_INTRO_2);
        z_game_log(z_game, Z_LOG_NEUTRAL, Z_STR_INTRO_3, Z_MONTHS_PER_TURN);
        z_game_log(z_game, Z_LOG_NEUTRAL, Z_STR_INTRO_4);
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
                    z_game_setInstructions(z_game, Z_STR_TURN_NEXT);
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
        z_game_log(z_game, Z_LOG_BAD, Z_STR_GAME_OVER);
        a_state_push("flushLog");
    }

    A_STATE_BODY
    {
        z_controls_release();
        z_game_setInstructions(z_game, Z_STR_GAME_NEW);

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
                z_game_setInstructions(z_game, Z_STR_GAME_WAIT);
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
