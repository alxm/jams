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

#include "util_despot.h"
#include "util_game.h"
#include "util_sfx.h"

#include "state_game.h"

static bool game_handleMenu(ZGame* Game)
{
    AMenu* menu = z_game_getMenu(Game);

    a_menu_handleInput(menu);

    if(a_menu_getState(menu) == A_MENU_STATE_SELECTED) {
        void* selected = a_menu_getSelectedItem(menu);

        a_menu_reset(menu);

        return z_game_runMenuHandler(Game, selected);
    }

    return false;
}

A_STATE(actionMenu)
{
    A_STATE_BODY
    {
        A_STATE_LOOP
        {
            z_game_logTick(z_game);

            z_game_setInstructions(z_game,
                                   "Choose an option with UP/DOWN and "
                                   "select with SPACE BAR");

            if(game_handleMenu(z_game)) {
                a_state_pop();
                a_state_push("flushLog");
            }

            A_STATE_LOOP_DRAW
            {
                z_game_draw(z_game);
                z_game_drawMenu(z_game);
            }
        }
    }

    A_STATE_FREE
    {
        z_game_logDec(z_game);
    }
}

bool z_action_doNothing(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    z_game_log(Game, Z_LOG_GOOD, "Despot got some well-earned rest");
    z_game_logInc(Game);
    z_despot_setHealth(despot, z_despot_getHealth(despot) + 1);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 1);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 1);
    z_game_logDec(Game);

    return true;
}

bool z_action_collectTaxes(ZGame* Game)
{
    z_game_setMenu(Game, Z_MENU_TAX);

    return false;
}

bool z_action_collectTaxesFromPeasants(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    z_game_log(Game, Z_LOG_GOOD, "Despot taxed the peasants, they'll like that");
    z_game_logInc(Game);
    z_despot_setWealth(despot, z_despot_getWealth(despot) + 100);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 1);
    z_game_logDec(Game);

    z_game_setMenu(Game, Z_MENU_MAIN);

    return true;
}

bool z_action_collectTaxesFromNobles(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    z_game_log(Game, Z_LOG_GOOD, "Despot taxed the nobles, they'll like that");
    z_game_logInc(Game);
    z_despot_setWealth(despot, z_despot_getWealth(despot) + 100);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 1);
    z_game_logDec(Game);

    z_game_setMenu(Game, Z_MENU_MAIN);

    return true;
}

bool z_action_giveMoney(ZGame* Game)
{
    z_game_setMenu(Game, Z_MENU_GIVE);

    return false;
}

bool z_action_giveMoneyToPeasants(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    z_game_log(Game, Z_LOG_GOOD, "Despot was generous to the peasants");
    z_game_logInc(Game);
    z_despot_setWealth(despot, z_despot_getWealth(despot) - 100);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) + 1);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 1);
    z_game_logDec(Game);

    z_game_setMenu(Game, Z_MENU_MAIN);

    return true;
}

bool z_action_giveMoneyToNobles(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    z_game_log(Game, Z_LOG_GOOD, "Despot was generous to the nobles");
    z_game_logInc(Game);
    z_despot_setWealth(despot, z_despot_getWealth(despot) - 100);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) + 1);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 1);
    z_game_logDec(Game);

    z_game_setMenu(Game, Z_MENU_MAIN);

    return true;
}

bool z_action_imprison(ZGame* Game)
{
    if(z_game_getNumImprisoned(Game) >= 2) {
        a_sfx_play(z_sfx.denied);

        z_game_log(Game,
                   Z_LOG_GOOD,
                   "Despot cannot imprison more people until next year");
    } else {
        z_game_setMenu(Game, Z_MENU_IMPRISON);
    }

    return false;
}

bool z_action_imprisonPeasants(ZGame* Game)
{
    bool ret = false;
    ZDespot* despot = z_game_getDespot(Game);

    if(z_despot_getLoyalty(despot) < 50) {
        z_game_log(Game, Z_LOG_GOOD, "Too many nobles oppose the Despot");
        z_game_logInc(Game);
        z_game_log(Game,
                   Z_LOG_GOOD,
                   "Cannot imprison peasants at this time (maybe just 1)");
        z_game_logDec(Game);

        goto done;
    }

    z_game_log(Game,
               Z_LOG_GOOD,
               "Despot imprisoned a group of rebellious peasants");

    z_game_logInc(Game);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 1);

    unsigned revoltCounter = a_math_minu(z_game_getRevoltCounter(Game), 1);
    z_game_setRevoltCounter(Game, revoltCounter);

    if(revoltCounter > 0) {
        z_game_log(Game,
                   Z_LOG_GOOD,
                   "Staved off revolt for another %d months",
                   Z_REVOLT_COUNT_MAX - 1);
    } else {
        z_game_log(Game, Z_LOG_GOOD, "A needless effort");
    }

    z_game_logDec(Game);

    z_game_setNumImprisoned(Game, z_game_getNumImprisoned(Game) + 1);
    ret = true;

done:
    z_game_setMenu(Game, Z_MENU_MAIN);
    return ret;
}

bool z_action_imprisonNobles(ZGame* Game)
{
    bool ret = false;
    ZDespot* despot = z_game_getDespot(Game);

    if(z_despot_getPopularity(despot) < 50) {
        z_game_log(Game, Z_LOG_GOOD, "Too many peasants oppose the Despot");
        z_game_logInc(Game);
        z_game_log(Game, Z_LOG_GOOD, "Cannot imprison nobles at this time");
        z_game_logDec(Game);

        goto done;
    }

    z_game_log(Game, Z_LOG_GOOD, "Despot imprisoned a corrupt noble");
    z_game_logInc(Game);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 1);

    unsigned coupCounter = a_math_minu(z_game_getCoupCounter(Game), 1);
    z_game_setCoupCounter(Game, coupCounter);

    if(coupCounter > 0) {
        z_game_log(Game,
                   Z_LOG_GOOD,
                   "Staved off coup for another %d months",
                   Z_COUP_COUNT_MAX - 1);
    } else {
        z_game_log(Game, Z_LOG_GOOD, "A needless effort");
    }

    z_game_logDec(Game);

    z_game_setNumImprisoned(Game, z_game_getNumImprisoned(Game) + 1);
    ret = true;

done:
    z_game_setMenu(Game, Z_MENU_MAIN);
    return ret;
}

bool z_action_wageWar(ZGame* Game)
{
    unsigned numWars = z_game_getNumWars(Game);

    if(numWars >= 1) {
        a_sfx_play(z_sfx.denied);

        z_game_log(Game,
                   Z_LOG_GOOD,
                   "Already waged a war this year, time flies huh");

        return false;
    }

    ZDespot* despot = z_game_getDespot(Game);

    int popularity = z_despot_getPopularity(despot);
    int loyalty = z_despot_getLoyalty(despot);
    int wealth = z_despot_getWealth(despot);
    int health = z_despot_getHealth(despot);

    if(popularity + loyalty < 50) {
        z_game_log(Game, Z_LOG_GOOD, "Too many people oppose the Despot");
        z_game_logInc(Game);
        z_game_log(Game, Z_LOG_GOOD, "Cannot wage war at this time");
        z_game_logDec(Game);

        return false;
    }

    int chance = 1 + (popularity + loyalty) / 10;

    z_game_log(Game,
               Z_LOG_GOOD,
               "Despot is waging war with %d%% chance of success",
               100 * chance / 21);

    z_game_logInc(Game);

    if(a_random_chance(chance, 21)) {
        z_game_log(Game,
                   Z_LOG_GOOD,
                   "Despot was victorious! GLORY TO THE DESPOT!");

        z_game_logInc(Game);

        if(popularity > 33) {
            z_despot_setPopularity(despot, popularity + 1);
        } else {
            z_despot_setPopularity(despot, popularity - 1);
        }

        z_despot_setWealth(despot, wealth + 1000);
        z_game_logDec(Game);
    } else {
        z_game_log(Game,
                   Z_LOG_GOOD,
                   "Despot was defeated, but it was YOUR fault");

        z_game_logInc(Game);
        z_despot_setPopularity(despot, popularity - 1);
        z_despot_setLoyalty(despot, loyalty - 1);
        z_despot_setWealth(despot, wealth - 1000);
        z_despot_setHealth(despot, health - 1);
        z_game_logDec(Game);
    }

    z_game_logDec(Game);

    z_game_setNumWars(Game, numWars + 1);

    return true;
}
