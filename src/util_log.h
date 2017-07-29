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

typedef struct ZLog ZLog;

extern ZLog* z_log_new(unsigned MaxLines);
extern void z_log_free(ZLog* Log);

extern void z_log_log(ZLog* Log, AFont* Font, const char* Format, va_list Args);

extern bool z_log_tick(ZLog* Log);
extern void z_log_draw(const ZLog* Log, int X, int Y);
