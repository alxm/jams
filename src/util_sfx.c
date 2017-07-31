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

#include "util_sfx.h"

ZSfx z_sfx;

void z_sfx_load(void)
{
    z_sfx.menuBrowse = a_sfx_newFromFile("sfx/menu_browse.wav");
    z_sfx.menuSelect = a_sfx_newFromFile("sfx/menu_select.wav");
    z_sfx.newTurn = a_sfx_newFromFile("sfx/new_turn.wav");
}
