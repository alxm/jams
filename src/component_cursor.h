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

typedef struct ZCompCursor ZCompCursor;

extern size_t z_comp_cursor_size(void);
extern void z_comp_cursor_init(ZCompCursor* Cursor);
extern AFree z_comp_cursor_free;

extern AEntity* z_comp_cursor_getHover(const ZCompCursor* Cursor);
extern void z_comp_cursor_setHover(ZCompCursor* Cursor, AEntity* Unit);

extern AEntity* z_comp_cursor_getSelected(const ZCompCursor* Cursor);
extern void z_comp_cursor_setSelected(ZCompCursor* Cursor, AEntity* Unit);
