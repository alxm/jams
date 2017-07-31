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
#include "util_strings.h"

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
            z_game_setInstructions(z_game, Z_STR_MENU_HELP);

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

    z_game_log(Game, Z_LOG_GOOD, Z_STR_ACT_NOTHING);
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

    z_game_log(Game, Z_LOG_NEUTRAL, Z_STR_ACT_TAX_PEASANTS);
    z_game_logInc(Game);
    z_despot_setWealth(despot, z_despot_getWealth(despot) + 1000);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 4);
    z_game_logDec(Game);

    z_game_setMenu(Game, Z_MENU_MAIN);

    return true;
}

bool z_action_collectTaxesFromNobles(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    z_game_log(Game, Z_LOG_NEUTRAL, Z_STR_ACT_TAX_NOBLES);
    z_game_logInc(Game);
    z_despot_setWealth(despot, z_despot_getWealth(despot) + 1000);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 4);
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

    z_game_log(Game, Z_LOG_GOOD, Z_STR_ACT_GIVE_PEASANTS);
    z_game_logInc(Game);
    z_despot_setWealth(despot, z_despot_getWealth(despot) - 1000);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) + 4);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 2);
    z_game_logDec(Game);

    z_game_setMenu(Game, Z_MENU_MAIN);

    return true;
}

bool z_action_giveMoneyToNobles(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    z_game_log(Game, Z_LOG_GOOD, Z_STR_ACT_GIVE_NOBLES);
    z_game_logInc(Game);
    z_despot_setWealth(despot, z_despot_getWealth(despot) - 1000);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) + 4);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 2);
    z_game_logDec(Game);

    z_game_setMenu(Game, Z_MENU_MAIN);

    return true;
}

bool z_action_imprison(ZGame* Game)
{
    if(z_game_getNumImprisoned(Game) >= 2) {
        a_sfx_play(z_sfx.denied);
        z_game_log(Game, Z_LOG_NEUTRAL, Z_STR_ACT_IMP_LIMIT);
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
        z_game_log(Game, Z_LOG_BAD, Z_STR_ACT_IMP_P_FAIL);
        z_game_logInc(Game);
        z_game_log(Game, Z_LOG_BAD, Z_STR_ACT_IMP_P_FAIL_INFO);
        z_game_logDec(Game);

        goto done;
    }

    z_game_log(Game, Z_LOG_GOOD, Z_STR_ACT_IMP_P_SUCCESS);
    z_game_logInc(Game);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 10);

    unsigned revoltCounter = a_math_minu(z_game_getRevoltCounter(Game), 1);
    z_game_setRevoltCounter(Game, revoltCounter);

    if(revoltCounter > 0) {
        z_game_log(Game, Z_LOG_GOOD, Z_STR_ACT_IMP_P_STAVED);
    } else {
        z_game_log(Game, Z_LOG_NEUTRAL, Z_STR_ACT_IMP_P_NEEDLESS);
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
        z_game_log(Game, Z_LOG_BAD, Z_STR_ACT_IMP_N_FAIL);
        z_game_logInc(Game);
        z_game_log(Game, Z_LOG_BAD, Z_STR_ACT_IMP_N_FAIL_INFO);
        z_game_logDec(Game);

        goto done;
    }

    z_game_log(Game, Z_LOG_GOOD, Z_STR_ACT_IMP_N_SUCCESS);
    z_game_logInc(Game);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 10);

    unsigned coupCounter = a_math_minu(z_game_getCoupCounter(Game), 1);
    z_game_setCoupCounter(Game, coupCounter);

    if(coupCounter > 0) {
        z_game_log(Game, Z_LOG_GOOD, Z_STR_ACT_IMP_N_STAVED);
    } else {
        z_game_log(Game, Z_LOG_NEUTRAL, Z_STR_ACT_IMP_N_NEEDLESS);
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
        z_game_log(Game, Z_LOG_NEUTRAL, Z_STR_ACT_WAR_LIMIT);

        return false;
    }

    ZDespot* despot = z_game_getDespot(Game);

    int popularity = z_despot_getPopularity(despot);
    int loyalty = z_despot_getLoyalty(despot);
    int wealth = z_despot_getWealth(despot);
    int health = z_despot_getHealth(despot);

    if(popularity + loyalty < 50) {
        z_game_log(Game, Z_LOG_BAD, Z_STR_ACT_WAR_FAIL);
        z_game_logInc(Game);
        z_game_log(Game, Z_LOG_BAD, Z_STR_ACT_WAR_FAIL_INFO);
        z_game_logDec(Game);

        return false;
    }

    int chance = 1 + (popularity + loyalty) / 10;
    int total =  1 + (100        + 100)     / 10;

    z_game_log(Game, Z_LOG_NEUTRAL, Z_STR_ACT_WAR_PROB, 100 * chance / total);
    z_game_logInc(Game);

    if(a_random_chance(chance, total)) {
        z_game_log(Game, Z_LOG_GOOD, Z_STR_ACT_WAR_VICTORY);
        z_game_logInc(Game);

        if(popularity > 33) {
            z_despot_setPopularity(despot, popularity + 10);
        } else {
            z_despot_setPopularity(despot, popularity / 2);
        }

        z_despot_setWealth(despot, wealth + 20000);
        z_game_logDec(Game);
    } else {
        z_game_log(Game, Z_LOG_BAD, Z_STR_ACT_WAR_DEFEAT);
        z_game_logInc(Game);
        z_despot_setPopularity(despot, popularity / 2);
        z_despot_setLoyalty(despot, loyalty / 2);
        z_despot_setWealth(despot, wealth - 10000);
        z_despot_setHealth(despot, health - 20);
        z_game_setRevoltCounter(Game, z_game_getRevoltCounter(Game) + 2);
        z_game_setCoupCounter(Game, z_game_getCoupCounter(Game) + 2);
        z_game_logDec(Game);
    }

    z_game_logDec(Game);

    z_game_setNumWars(Game, numWars + 1);

    return true;
}
