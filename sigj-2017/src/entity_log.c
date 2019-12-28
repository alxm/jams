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

#include "component_log.h"

AEntity* z_entity_log_new(void)
{
    AEntity* e = a_entity_new("log", NULL);

    ZCompLog* log = a_entity_addComponent(e, "log");
    z_comp_log_init(log, 4);

    return e;
}

void z_entity_log_write(AEntity* Log, AFont* Font, int Indent, const char* Format, ...)
{
    ZCompLog* log = a_entity_requireComponent(Log, "log");

    va_list args;
    va_start(args, Format);

    z_comp_log_write(log, Font, Indent, Format, args);

    va_end(args);
}
