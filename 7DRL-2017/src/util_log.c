/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_log.h"

struct ZLog {
    AList* lines;
    unsigned maxLines;
};

ZLog* z_log_new(unsigned MaxLines)
{
    ZLog* l = a_mem_malloc(sizeof(ZLog));

    l->lines = a_list_new();
    l->maxLines = MaxLines;

    return l;
}

void z_log_free(ZLog* Log)
{
    A_LIST_ITERATE(Log->lines, char*, line) {
        free(line);
    }

    a_list_free(Log->lines);
    free(Log);
}

void z_log_log(ZLog* Log, const char* Format, va_list Args)
{
    va_list argsCopy;
    va_copy(argsCopy, Args);

    int needed = vsnprintf(NULL, 0, Format, argsCopy);

    va_end(argsCopy);

    if(needed > 0) {
        size_t size = (size_t)needed + 1;
        char* buffer = a_mem_malloc(size);
        needed = vsnprintf(buffer, size, Format, Args);

        if(needed > 0 && (size_t)needed < size) {
            a_list_addLast(Log->lines, buffer);
            a_out_printf(buffer);

            if(a_list_getSize(Log->lines) > Log->maxLines) {
                free(a_list_pop(Log->lines));
            }
        } else {
            free(buffer);
        }
    }
}

AList* z_log_getLines(const ZLog* Log)
{
    return Log->lines;
}
