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

typedef struct ZLog ZLog;

extern ZLog* z_log_new(unsigned MaxLines);
extern void z_log_free(ZLog* Log);

extern void z_log_log(ZLog* Log, unsigned Font, const char* Format, va_list Args);

extern bool z_log_tick(ZLog* Log);
extern void z_log_draw(const ZLog* Log, int X, int Y);
