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

#define Z_LOAD_FROM_DATA

#ifdef Z_LOAD_FROM_DATA
    #include "a2x_gen/sfx.h"
#endif

#include "util_sfx.h"

ZSfx z_sfx;

void z_sfx_load(void)
{
    #ifdef Z_LOAD_FROM_DATA
        z_sfx.menuBrowse = a_sfx_newFromData(sfx_menu_browse, sfx_menu_browse_size);
        z_sfx.menuSelect = a_sfx_newFromData(sfx_menu_select, sfx_menu_select_size);
        z_sfx.newTurn = a_sfx_newFromData(sfx_new_turn, sfx_new_turn_size);
        z_sfx.outcomeGood = a_sfx_newFromData(sfx_outcome_good, sfx_outcome_good_size);
        z_sfx.outcomeBad = a_sfx_newFromData(sfx_outcome_bad, sfx_outcome_bad_size);
        z_sfx.denied = a_sfx_newFromData(sfx_denied, sfx_denied_size);
    #else
        z_sfx.menuBrowse = a_sfx_newFromFile("sfx/menu_browse.wav");
        z_sfx.menuSelect = a_sfx_newFromFile("sfx/menu_select.wav");
        z_sfx.newTurn = a_sfx_newFromFile("sfx/new_turn.wav");
        z_sfx.outcomeGood = a_sfx_newFromFile("sfx/outcome_good.wav");
        z_sfx.outcomeBad = a_sfx_newFromFile("sfx/outcome_bad.wav");
        z_sfx.denied = a_sfx_newFromFile("sfx/denied.wav");
    #endif
}
