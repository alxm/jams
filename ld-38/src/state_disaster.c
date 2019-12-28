/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    38th Century - A simple world simulation game made for Ludum Dare 38 72h

    38th Century is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    38th Century is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 38th Century.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_data.h"
#include "util_graphics.h"
#include "util_log.h"
#include "util_period.h"

#include "state_game.h"

A_STATE(disaster)
{
    A_STATE_INIT
    {
        ZDataType data = 0;
        const char* message = NULL;

        switch(z_game.disaster.type) {
            case Z_DISASTER_DISEASE: {
                data = Z_DATA_HEALTH;
                message = "A plague hit the population";
            } break;

            case Z_DISASTER_CALAMITY: {
                data = Z_DATA_INDUSTRY;
                message = "An earthquake shook the land";
            } break;

            case Z_DISASTER_ATTACK: {
                data = Z_DATA_DEFENSE;
                message = "Aliens zapped your world";
            } break;

            default: break;
        }

        AFix died = 0;
        AFix population = z_data_getReal(z_game.data, Z_DATA_POPULATION);
        int oldValue = z_data_getReal(z_game.data, data);
        int newValue = oldValue - z_game.disaster.damage;

        if(newValue < 0) {
            died = a_math_min(a_fix_itofix(-newValue) / 4, population);
            newValue = 0;
        }

        z_data_inc(z_game.data, data, -z_game.disaster.damage);

        z_game_log(z_fonts.lightGray, message);
        z_game_log(z_fonts.lightGray,
                   "  Lost %d %s points",
                   oldValue - newValue,
                   z_data_getName(data));

        if(a_fix_fixtoi(died) > 0) {
            z_game_log(z_fonts.magenta,
                       "  %d people died (%d%%)",
                       a_fix_fixtoi(died),
                       100 * died / population);
        } else {
            z_game_log(z_fonts.lightGray, "  Everyone survived!");
        }

        z_game_kill(died);
    }

    A_STATE_BODY
    {
        A_STATE_LOOP
        {
            if(z_game_tick()) {
                a_state_pop();
            }

            A_STATE_LOOP_DRAW
            {
                z_game_draw(Z_HELP_MAINGAME);
            }
        }
    }

    A_STATE_FREE
    {
        z_game.disaster.type = a_random_int(Z_DISASTER_NUM);
        z_game.disaster.damage = 64;
    }
}
