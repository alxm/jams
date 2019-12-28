/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_fonts.h"

#include "component_log.h"

typedef struct ZCompLogLine {
    char* text;
    AFont* font;
    int indent;
} ZCompLogLine;

struct ZCompLog {
    AList* lines; // list of ZCompLogLine
    AList* backlog;
    unsigned maxLines;
};

static void line_free(ZCompLogLine* Line)
{
    free(Line->text);
    free(Line);
}

size_t z_comp_log_size(void)
{
    return sizeof(ZCompLog);
}

void z_comp_log_init(ZCompLog* Log, unsigned MaxLines)
{
    Log->lines = a_list_new();
    Log->backlog = a_list_new();
    Log->maxLines = MaxLines;
}

void z_comp_log_free(void* Self)
{
    ZCompLog* log = Self;

    a_list_freeEx(log->lines, (AListFree*)line_free);
    a_list_freeEx(log->backlog, (AListFree*)line_free);
}

void z_comp_log_scroll(ZCompLog* Log)
{
    ZCompLogLine* newLine = a_list_pop(Log->backlog);

    if(newLine == NULL) {
        return;
    }

    a_list_addLast(Log->lines, newLine);

    if(a_list_getSize(Log->lines) > Log->maxLines) {
        line_free(a_list_pop(Log->lines));
    }
}

void z_comp_log_draw(const ZCompLog* Log)
{
    a_font_setCoords(8, 8);

    int alphaInc = (3 * A_PIXEL_ALPHA_MAX / 4) / (int)Log->maxLines;
    int numLines = (int)a_list_getSize(Log->lines);
    int alpha = A_PIXEL_ALPHA_MAX - (numLines + 1) * alphaInc;

    a_pixel_push();
    a_pixel_setBlend(A_PIXEL_BLEND_RGBA);

    A_LIST_ITERATE(Log->lines, ZCompLogLine*, line) {
        if(A_LIST_IS_LAST()) {
            a_pixel_setBlend(A_PIXEL_BLEND_PLAIN);
        } else {
            a_pixel_setAlpha(alpha);
            alpha += alphaInc;
        }

        for(int i = 0; i < line->indent; i++) {
            a_font_print("  ");
        }

        a_font_setFont(z_util_fonts.gray3);
        a_font_print("> ");
        a_font_setFont(line->font);
        a_font_print(line->text);
        a_font_newLine();
    }

    a_pixel_pop();
}

void z_comp_log_write(ZCompLog* Log, AFont* Font, int Indent, const char* Format, va_list Args)
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
            a_out_print(buffer);

            ZCompLogLine* line = a_mem_malloc(sizeof(ZCompLogLine));

            line->text = buffer;
            line->font = Font;
            line->indent = Indent;

            a_list_addLast(Log->backlog, line);
        } else {
            free(buffer);
        }
    }
}
