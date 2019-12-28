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

#include "util_level.h"

static AList* g_levels;

void u_level_load(void)
{
    g_levels = a_list_new();

    ADir* root = a_dir_new("assets/levels");

    A_LIST_ITERATE(a_dir_entriesListGet(root), APath*, p) {
        if(a_path_test(p, A_PATH_FILE)) {
            a_list_addLast(g_levels, a_block_new(a_path_getFull(p)));
        }
    }

    a_dir_free(root);
}

void u_level_unload(void)
{
    a_list_free(g_levels);
}

const ABlock* u_level_get(unsigned Index)
{
    return a_list_getByIndex(g_levels, Index);
}
