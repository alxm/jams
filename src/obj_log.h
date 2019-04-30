/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of Coffin Digital, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <a2x.h>

#include "util_font.h"

extern void n_log_new(void);
extern void n_log_free(void);

extern void n_log_write(UFontId FontMain, UFontId FontHighlight, const char* Format, ...);
extern bool n_log_done(void);

extern void n_log_inc(void);
extern void n_log_dec(void);

extern void n_log_tick(void);
extern void n_log_draw(void);
