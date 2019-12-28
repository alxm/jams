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

#pragma once

#include <a2x.h>

#include "util_font.h"

extern void u_log_load(void);
extern void u_log_unload(void);

extern void u_log_add(UFontId Font, const char* Format, ...);

extern void u_log_inc(void);
extern void u_log_dec(void);

extern void u_log_tick(void);
extern void u_log_draw(int X, int Y);
