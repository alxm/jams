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

#include "state_load.h"

#include "util_controls.h"
#include "util_ecs.h"
#include "util_font.h"
#include "util_level.h"
#include "util_log.h"
#include "util_map.h"
#include "util_state.h"

A_STATE(t_load)
{
    static struct {
        void (*load)(void);
        void (*unload)(void);
        bool loaded;
    } table[] = {
        {u_controls_load, u_controls_unload, false},
        {u_ecs_load, NULL, false},
        {u_font_load, u_font_unload, false},
        {u_level_load, u_level_unload, false},
        {u_log_load, u_log_unload, false},
        {u_map_load, u_map_unload, false},
    };

    A_STATE_INIT
    {
        for(unsigned i = 0; i < A_ARRAY_LEN(table); i++) {
            if(table[i].load) {
                table[i].load();
                table[i].loaded = true;
            }
        }
    }

    A_STATE_TICK
    {
        u_state_push(U_STATE_ID_GAME);
    }

    A_STATE_FREE
    {
        for(unsigned i = A_ARRAY_LEN(table); i--; ) {
            if(table[i].unload && table[i].loaded) {
                table[i].unload();
                table[i].loaded = false;
            }
        }
    }
}
