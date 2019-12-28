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
#include "obj_visitor.h"
#include "util_font.h"
#include "util_input.h"

typedef enum {
    Z_COMMAND_INVALID = -1,
    Z_COMMAND_MESSAGE,
    Z_COMMAND_MESSAGE_BREAK,
    Z_COMMAND_VISITOR,
    Z_COMMAND_WAIT_MS,
    Z_COMMAND_WAIT_SPACE,
    Z_COMMAND_NUM
} ZCommand;

typedef struct {
    ZCommand command;
    union {
        const char* buffer;
        unsigned millis;
        NVisitorId visitor;
    } context;
} ZEvent;

static const ZEvent g_events[] = {
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "Hello and welcome to `Coffin Digital`, a `sanctioned company`."},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "As `Resource Manager`, your goal is to source assets"},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "that will grow our resource pool."},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "Your lease term is 2 weeks, proceed."},
    {.command = Z_COMMAND_MESSAGE_BREAK},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "..."},
    {.command = Z_COMMAND_WAIT_MS, .context.millis = 400},
    {.command = Z_COMMAND_WAIT_SPACE},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "Your first visitor."},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "This is our chance to see if this asset has the resources"},
    {.command = Z_COMMAND_MESSAGE, .context.buffer = "we seek to add to our pool"},
    {.command = Z_COMMAND_MESSAGE_BREAK},
    {.command = Z_COMMAND_VISITOR, .context.visitor = N_VISITOR_0},
    {.command = Z_COMMAND_INVALID},
};

static struct {
    int flag;
    ATimer* timer;
} g_context;

static unsigned g_index;

static void eventDone(void)
{
    g_index++;

    g_context.flag = 0;

    a_timer_stop(g_context.timer);
}

void n_event_new(void)
{
    g_index = 0;

    g_context.timer = a_timer_new(A_TIMER_MS, 0, false);
}

void n_event_free(void)
{
    a_timer_free(g_context.timer);
}

void n_event_tick(void)
{
    const ZEvent* e = &g_events[g_index];

    switch(e->command) {
        case Z_COMMAND_MESSAGE: {
            switch(g_context.flag) {
                case 0: {
                    g_context.flag++;

                    n_log_write(U_FONT_GRAY_LIGHT,
                                U_FONT_YELLOW,
                                "%s",
                                e->context.buffer);
                } break;

                case 1: {
                    if(n_log_done()) {
                        eventDone();
                    }
                } break;
            }
        } break;

        case Z_COMMAND_MESSAGE_BREAK: {
            switch(g_context.flag) {
                case 0: {
                    g_context.flag++;

                    n_log_break();
                } break;

                case 1: {
                    if(n_log_done()) {
                        eventDone();
                    }
                } break;
            }
        } break;

        case Z_COMMAND_VISITOR: {
            switch(g_context.flag) {
                case 0: {
                    g_context.flag++;

                    n_visitor_new();
                } break;

                case 1: {
                    g_context.flag++;

                    n_log_write(U_FONT_GRAY_LIGHT,
                                U_FONT_YELLOW,
                                "Hello there, I am %s",
                                n_visitor_nameGet());
                } break;
            }
        } break;

        case Z_COMMAND_WAIT_MS: {
            if(a_timer_expiredGet(g_context.timer)) {
                eventDone();
            } else if(!a_timer_isRunning(g_context.timer)) {
                a_timer_periodSet(g_context.timer, e->context.millis);
                a_timer_start(g_context.timer);
            }
        } break;

        case Z_COMMAND_WAIT_SPACE: {
            switch(g_context.flag) {
                case 0: {
                    g_context.flag++;

                    n_log_break();
                    n_log_write(U_FONT_GRAY_LIGHT,
                                U_FONT_PINK,
                                "Press `SPACE` to continue");

                    a_button_pressClear(u_input_get(U_BUTTON_ACTION));
                } break;

                case 1: {
                    if(a_button_pressGet(u_input_get(U_BUTTON_ACTION))) {
                        g_context.flag++;
                    }
                } break;

                case 2: {
                    g_context.flag++;

                    n_log_break();
                } break;

                case 3: {
                    if(n_log_done()) {
                        g_context.flag++;

                        eventDone();
                    }
                } break;
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
