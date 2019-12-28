/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Cave Shrine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cave Shrine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cave Shrine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "util_state.h"

#include "state_game.h"
#include "state_load.h"

static const struct {
    AState* function;
    const char* name;
} g_states[U_STATE_ID_NUM] = {
    [U_STATE_ID_LOAD] = {t_load, "Loading"},
    [U_STATE_ID_GAME] = {t_game, "Game"},
};

AState* u_state_get(UStateId Id)
{
    return g_states[Id].function;
}

void u_state_push(UStateId Id)
{
    a_state_push(g_states[Id].function, g_states[Id].name);
}

void u_state_replace(UStateId Id)
{
    a_state_replace(g_states[Id].function, g_states[Id].name);
}
