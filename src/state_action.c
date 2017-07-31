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

#include "state_game.h"

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

            if(z_game_handleMenu(z_game)) {
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

    z_game_log(Game, NULL, "Despot got some well-earned rest");
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

    z_game_log(Game, NULL, "Despot taxed the peasants");
    z_game_logInc(Game);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 1);
    z_despot_setWealth(despot, z_despot_getWealth(despot) + 100);
    z_game_logDec(Game);

    z_game_setMenu(Game, Z_MENU_MAIN);

    return true;
}

bool z_action_collectTaxesFromNobles(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    z_game_log(Game, NULL, "Despot taxed the nobles");
    z_game_logInc(Game);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 1);
    z_despot_setWealth(despot, z_despot_getWealth(despot) + 100);
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

    z_game_log(Game, NULL, "Despot was generous to the peasants");
    z_game_logInc(Game);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) + 1);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 1);
    z_despot_setWealth(despot, z_despot_getWealth(despot) - 100);
    z_game_logDec(Game);

    z_game_setMenu(Game, Z_MENU_MAIN);

    return true;
}

bool z_action_giveMoneyToNobles(ZGame* Game)
{
    ZDespot* despot = z_game_getDespot(Game);

    z_game_log(Game, NULL, "Despot was generous to the nobles");
    z_game_logInc(Game);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 1);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) + 1);
    z_despot_setWealth(despot, z_despot_getWealth(despot) - 100);
    z_game_logDec(Game);

    z_game_setMenu(Game, Z_MENU_MAIN);

    return true;
}

bool z_action_imprison(ZGame* Game)
{
    z_game_setMenu(Game, Z_MENU_IMPRISON);

    return false;
}

bool z_action_imprisonPeasants(ZGame* Game)
{
    bool ret = false;
    unsigned numImprisoned = z_game_getNumImprisoned(Game);

    if(numImprisoned++ >= 2) {
        z_game_log(Game,
                   NULL,
                   "Despot cannot imprison people again until next year");

        goto done;
    }

    ZDespot* despot = z_game_getDespot(Game);

    if(z_despot_getLoyalty(despot) < 50) {
        z_game_log(Game, NULL, "Too many nobles oppose the Despot");
        z_game_logInc(Game);
        z_game_log(Game,
                   NULL,
                   "Cannot imprison peasants at this time (maybe just 1)");
        z_game_logDec(Game);

        goto done;
    }

    z_game_log(Game, NULL, "Despot imprisoned a group of rebellious peasants");
    z_game_logInc(Game);
    z_despot_setPopularity(despot, z_despot_getPopularity(despot) - 1);
    z_game_staveOffRevolt(Game);
    z_game_logDec(Game);

    z_game_setNumImprisoned(Game, numImprisoned);
    ret = true;

done:
    z_game_setMenu(Game, Z_MENU_MAIN);
    return ret;
}

bool z_action_imprisonNobles(ZGame* Game)
{
    bool ret = false;
    unsigned numImprisoned = z_game_getNumImprisoned(Game);

    if(numImprisoned++ >= 2) {
        z_game_log(Game,
                   NULL,
                   "Despot cannot imprison people again until next year");

        goto done;
    }

    ZDespot* despot = z_game_getDespot(Game);

    if(z_despot_getPopularity(despot) < 50) {
        z_game_log(Game, NULL, "Too many peasants oppose the Despot");
        z_game_logInc(Game);
        z_game_log(Game, NULL, "Cannot imprison nobles at this time");
        z_game_logDec(Game);

        goto done;
    }

    z_game_log(Game, NULL, "Despot imprisoned a corrupt noble");
    z_game_logInc(Game);
    z_despot_setLoyalty(despot, z_despot_getLoyalty(despot) - 1);
    z_game_staveOffCoup(Game);
    z_game_logDec(Game);

    z_game_setNumImprisoned(Game, numImprisoned);
    ret = true;

done:
    z_game_setMenu(Game, Z_MENU_MAIN);
    return ret;
}

bool z_action_wageWar(ZGame* Game)
{
    A_UNUSED(Game);
    return true;
}
