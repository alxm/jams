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

#include "obj_log.h"

#include "util_gfx.h"

typedef struct {
    AList* lines;
    AList* backlog;
    unsigned maxLines;
    int indent;
    ATimer* updateDelay;
} NLog;

typedef struct NLogLine {
    UFontId fonts[2];
    int indent;
    char* text;
    AList* chunks;
} NLogLine;

static NLog g_log;

static NLogLine* lineNew(UFontId FontMain, UFontId FontHighlight, int Indent, char* Buffer)
{
    NLogLine* line = a_mem_malloc(sizeof(NLogLine));

    line->fonts[0] = FontMain;
    line->fonts[1] = FontHighlight;
    line->indent = Indent;
    line->text = Buffer;
    line->chunks = a_str_split(Buffer, "`");

    return line;
}

static void lineFree(NLogLine* Line)
{
    a_list_freeEx(Line->chunks, free);

    free(Line->text);
    free(Line);
}

void n_log_new(void)
{
    g_log.lines = a_list_new();
    g_log.backlog = a_list_new();
    g_log.maxLines = 8;
    g_log.indent = 0;
    g_log.updateDelay = a_timer_new(A_TIMER_MS, 200, false);
}

void n_log_free(void)
{
    a_list_freeEx(g_log.lines, (AFree*)lineFree);
    a_list_freeEx(g_log.backlog, (AFree*)lineFree);

    a_timer_free(g_log.updateDelay);
}

void n_log_write(UFontId FontMain, UFontId FontHighlight, const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    int needed = vsnprintf(NULL, 0, Format, args);

    va_end(args);

    if(needed >= 0) {
        size_t size = (size_t)needed + 1;
        char* buffer = a_mem_malloc(size);

        va_list args;
        va_start(args, Format);

        needed = vsnprintf(buffer, size, Format, args);

        va_end(args);

        if(needed >= 0 && (size_t)needed < size) {
            a_list_addLast(
                g_log.backlog,
                lineNew(FontMain, FontHighlight, g_log.indent, buffer));

            a_out_text(buffer);

            if(!a_timer_isRunning(g_log.updateDelay)) {
                a_timer_start(g_log.updateDelay);
            }
        } else {
            free(buffer);
        }
    }
}

void n_log_break(void)
{
    n_log_write(U_FONT_INVALID, U_FONT_INVALID, "");
}

bool n_log_done(void)
{
    return a_list_isEmpty(g_log.backlog);
}

void n_log_inc(void)
{
    g_log.indent++;
}

void n_log_dec(void)
{
    g_log.indent = a_math_max(g_log.indent - 1, 0);
}

void n_log_tick(void)
{
    if(!a_list_peek(g_log.backlog) || !a_timer_expiredGet(g_log.updateDelay)) {
        return;
    }

    NLogLine* newLine = a_list_pop(g_log.backlog);
    a_list_addLast(g_log.lines, newLine);

    if(a_list_sizeGet(g_log.lines) > g_log.maxLines) {
        lineFree(a_list_pop(g_log.lines));
    }

    if(a_list_peek(g_log.backlog)) {
        a_timer_start(g_log.updateDelay);
    }
}

void n_log_draw(void)
{
    a_font_coordsSet(76, 181);

    A_LIST_ITERATE(g_log.lines, const NLogLine*, line) {
        for(int i = 0; i < line->indent; i++) {
            a_font_print("  ");
        }

        int highlight = 0;

        A_LIST_ITERATE(line->chunks, const char*, text) {
            a_font_fontSet(u_font_get(line->fonts[highlight]));
            a_font_print(text);

            if(!A_LIST_IS_LAST()) {
                highlight = 1 - highlight;
            }
        }

        a_font_lineHeightSet(10);
        a_font_newLine();
    }
}
