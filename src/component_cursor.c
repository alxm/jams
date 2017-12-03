/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_cursor.h"

struct ZCompCursor {
    int x;
};

size_t z_comp_cursor_size(void)
{
    return sizeof(ZCompCursor);
}

void z_comp_cursor_init(ZCompCursor* Cursor)
{
    A_UNUSED(Cursor);
}

void z_comp_cursor_free(void* Self)
{
    ZCompCursor* cursor = Self;

    A_UNUSED(cursor);
}
