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

#include <a2x.h>

#include "util_log.h"

struct ZLog {
    AList* lines;
    AList* backlog;
    unsigned maxLines;
};

typedef struct ZLogLine {
    char* text;
    AFont* font;
} ZLogLine;

ZLog* z_log_new(unsigned MaxLines)
{
    ZLog* l = a_mem_malloc(sizeof(ZLog));

    l->lines = a_list_new();
    l->backlog = a_list_new();
    l->maxLines = MaxLines;

    return l;
}

void z_log_free(ZLog* Log)
{
    A_LIST_ITERATE(Log->lines, ZLogLine*, line) {
        free(line->text);
        free(line);
    }

    A_LIST_ITERATE(Log->backlog, ZLogLine*, line) {
        free(line->text);
        free(line);
    }

    a_list_free(Log->lines);
    a_list_free(Log->backlog);
    free(Log);
}

void z_log_log(ZLog* Log, AFont* Font, const char* Format, va_list Args)
{
    va_list argsCopy;
    va_copy(argsCopy, Args);

    int needed = vsnprintf(NULL, 0, Format, argsCopy);

    va_end(argsCopy);

    if(needed >= 0) {
        size_t size = (size_t)needed + 1;
        char* buffer = a_mem_malloc(size);
        needed = vsnprintf(buffer, size, Format, Args);

        if(needed >= 0 && (size_t)needed < size) {
            a_out_printf(buffer);

            ZLogLine* line = a_mem_malloc(sizeof(ZLogLine));
            line->text = buffer;
            line->font = Font;

            a_list_addLast(Log->backlog, line);
        } else {
            free(buffer);
        }
    }
}

bool z_log_tick(ZLog* Log)
{
    ZLogLine* newLine = a_list_pop(Log->backlog);

    if(newLine) {
        a_list_addLast(Log->lines, newLine);

        if(a_list_getSize(Log->lines) > Log->maxLines) {
            ZLogLine* line = a_list_pop(Log->lines);
            free(line->text);
            free(line);
        }

        return false;
    }

    return true;
}

void z_log_draw(const ZLog* Log, int X, int Y)
{
    a_font_setCoords(X, Y);

    int alphaInc = 20;
    int numLines = (int)a_list_getSize(Log->lines);
    int alpha = A_PIXEL_ALPHA_MAX - (numLines + 1) * alphaInc;

    a_pixel_push();
    a_pixel_setBlend(A_PIXEL_BLEND_RGBA);

    A_LIST_ITERATE(Log->lines, ZLogLine*, line) {
        if(A_LIST_IS_LAST()) {
            a_pixel_setBlend(A_PIXEL_BLEND_PLAIN);
        } else {
            a_pixel_setAlpha(alpha);
            alpha += alphaInc;
        }

        a_font_setFont(NULL);
        a_font_print(">");
        a_font_setFont(line->font);
        a_font_print(line->text);
        a_font_newLine();
    }

    a_pixel_pop();
}
