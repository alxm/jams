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

void z_system_logTick(AEntity* Entity)
{
    ZCompLog* log = a_entity_requireComponent(Entity, "log");

    if(a_fps_isNthFrame(a_fps_msToFrames(200))) {
        z_comp_log_scroll(log);
    }
}

void z_system_logDraw(AEntity* Entity)
{
    ZCompLog* log = a_entity_requireComponent(Entity, "log");

    z_comp_log_draw(log);
}
