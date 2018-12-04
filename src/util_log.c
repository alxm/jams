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

#include "util_log.h"

typedef struct {
    AList* lines;
    AList* backlog;
    unsigned maxLines;
    int indent;
} ULog;

typedef struct ULogLine {
    UFontId font;
    int indent;
    char* text;
} ULogLine;

static ULog g_log;

static ULogLine* lineNew(UFontId Font, int Indent, char* Buffer)
{
    ULogLine* line = a_mem_malloc(sizeof(ULogLine));

    line->font = Font;
    line->indent = Indent;
    line->text = Buffer;

    return line;
}

static void lineFree(ULogLine* Line)
{
    free(Line->text);
    free(Line);
}

void u_log_load(void)
{
    g_log.lines = a_list_new();
    g_log.backlog = a_list_new();
    g_log.maxLines = 5;
    g_log.indent = 0;
}

void u_log_unload(void)
{
    a_list_freeEx(g_log.lines, (AFree*)lineFree);
    a_list_freeEx(g_log.backlog, (AFree*)lineFree);
}

void u_log_add(UFontId Font, const char* Format, ...)
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
            a_out_print(buffer);
        } else {
            free(buffer);
        }
    }
}

void u_log_inc(void)
{
    g_log.indent++;
}

void u_log_dec(void)
{
    g_log.indent = a_math_max(g_log.indent - 1, 0);
}

void u_log_tick(void)
{
    if(!a_list_peek(g_log.backlog) || !a_fps_ticksNth(a_time_msToTicks(200))) {
        return;
    }

    ULogLine* newLine = a_list_pop(g_log.backlog);
    a_list_addLast(g_log.lines, newLine);

    if(a_list_sizeGet(g_log.lines) > g_log.maxLines) {
        lineFree(a_list_pop(g_log.lines));
    }
}

void u_log_draw(int X, int Y)
{
    int alphaInc = (A_PIXEL_ALPHA_MAX * 3 / 4) / (int)g_log.maxLines;
    int numLines = (int)a_list_sizeGet(g_log.lines);
    int alpha = A_PIXEL_ALPHA_MAX - (numLines + 1) * alphaInc;

    a_pixel_push();
    a_pixel_blendSet(A_PIXEL_BLEND_RGBA);

    a_font_coordsSet(X, Y - (numLines - 1) * a_font_lineHeightGet());

    A_LIST_ITERATE(g_log.lines, const ULogLine*, line) {
        if(A_LIST_IS_LAST()) {
            a_pixel_blendSet(A_PIXEL_BLEND_PLAIN);
        } else {
            a_pixel_alphaSet(alpha);
            alpha += alphaInc;
        }

        for(int i = 0; i < line->indent; i++) {
            a_font_print("  ");
        }

        a_font_fontSet(u_font_get(U_FONT_RED));
        a_font_print("> ");

        a_font_fontSet(u_font_get(line->font));
        a_font_print(line->text);

        a_font_newLine();
    }

    a_pixel_pop();
}
