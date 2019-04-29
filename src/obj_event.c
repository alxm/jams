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

#include "obj_event.h"

#include "obj_log.h"
#include "util_font.h"

typedef enum {
    Z_COMMAND_INVALID = -1,
    Z_COMMAND_MESSAGE,
    Z_COMMAND_WAIT,
    Z_COMMAND_NUM
} ZCommand;

typedef struct {
    const ZCommand command;
    union {
        const char* buffer;
        unsigned millis;
    } context;
} ZEvent;

static ZEvent g_events[] = {
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "Hello and welcome to Coffin Digital"},
    {.command = Z_COMMAND_WAIT, .context.millis = 400},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "That's all I have."},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "That's all I have1."},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "That's all I have2."},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "That's all I have3."},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "That's all I have4."},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "That's all I have5."},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "That's all I have6."},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "That's all I have7."},
    {.command = Z_COMMAND_WAIT, .context.millis = 1000},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = ";-)"},
    {.command = Z_COMMAND_INVALID},
};

static unsigned g_index;
static ATimer* g_timer;

void n_event_new(void)
{
    g_index = 0;
    g_timer = a_timer_new(A_TIMER_MS, 0, false);
}

void n_event_free(void)
{
    a_timer_free(g_timer);
}

void n_event_tick(void)
{
    ZEvent* e = &g_events[g_index];

    switch(e->command) {
        case Z_COMMAND_MESSAGE: {
            if(e->context.buffer) {
                n_log_write(U_FONT_GRAY_LIGHT, "%s", e->context.buffer);
                e->context.buffer = NULL;
            } else if(n_log_done()) {
                g_index++;
            }
        } break;

        case Z_COMMAND_WAIT: {
            if(a_timer_expiredGet(g_timer)) {
                g_index++;
            } else if(!a_timer_isRunning(g_timer)) {
                a_timer_periodSet(g_timer, e->context.millis);
                a_timer_start(g_timer);
            }
        } break;

        default: {
            //
        } break;
    }
}

void n_event_draw(void)
{
    //
}
