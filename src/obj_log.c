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

typedef struct {
    AList* lines;
    AList* backlog;
    unsigned maxLines;
    int indent;
} NLog;

typedef struct NLogLine {
    UFontId font;
    int indent;
    char* text;
} NLogLine;

static NLog g_log;

static NLogLine* lineNew(UFontId Font, int Indent, char* Buffer)
{
    NLogLine* line = a_mem_malloc(sizeof(NLogLine));

    line->font = Font;
    line->indent = Indent;
    line->text = Buffer;

    return line;
}

static void lineFree(NLogLine* Line)
{
    free(Line->text);
    free(Line);
}

void n_log_new(void)
{
    g_log.lines = a_list_new();
    g_log.backlog = a_list_new();
    g_log.maxLines = 8;
    g_log.indent = 0;
}

void n_log_free(void)
{
    a_list_freeEx(g_log.lines, (AFree*)lineFree);
    a_list_freeEx(g_log.backlog, (AFree*)lineFree);
}

void n_log_write(UFontId Font, const char* Format, ...)
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
            a_list_addLast(g_log.backlog, lineNew(Font, g_log.indent, buffer));
            a_out_text(buffer);
        } else {
            free(buffer);
        }
    }
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
    if(!a_list_peek(g_log.backlog) || !a_fps_ticksNth(a_time_msToTicks(200))) {
        return;
    }

    NLogLine* newLine = a_list_pop(g_log.backlog);
    a_list_addLast(g_log.lines, newLine);

    if(a_list_sizeGet(g_log.lines) > g_log.maxLines) {
        lineFree(a_list_pop(g_log.lines));
    }
}

void n_log_draw(void)
{
    int X = 76;
    int Y = 181;
    int numLines = (int)a_list_sizeGet(g_log.lines);

    a_font_coordsSet(X, Y - (numLines - 1) * a_font_lineHeightGet());

    A_LIST_ITERATE(g_log.lines, const NLogLine*, line) {
        for(int i = 0; i < line->indent; i++) {
            a_font_print("  ");
        }

        a_font_fontSet(u_font_get(U_FONT_DEFAULT));
        a_font_print("> ");

        a_font_fontSet(u_font_get(line->font));
        a_font_print(line->text);

        a_font_newLine();
    }
}
