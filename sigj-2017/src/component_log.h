/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef struct ZCompLog ZCompLog;

extern size_t z_comp_log_size(void);
extern void z_comp_log_init(ZCompLog* Log, unsigned MaxLines);
extern AComponentFree z_comp_log_free;

extern void z_comp_log_scroll(ZCompLog* Log);
extern void z_comp_log_draw(const ZCompLog* Log);

extern void z_comp_log_write(ZCompLog* Log, AFont* Font, int Indent, const char* Format, va_list Args);
