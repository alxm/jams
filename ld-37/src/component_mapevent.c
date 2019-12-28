/*
    Copyright 2016 Alex Margarit

    This file is part of Motel 37.

    Motel 37 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Motel 37 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Motel 37.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "tiles.h"
#include "world.h"
#include "component_mapevent.h"

struct ZCompMapEvent {
    ZCompMapEventHandler* handler;
};

size_t z_comp_mapevent_size(void)
{
    return sizeof(ZCompMapEvent);
}

void z_comp_mapevent_init(ZCompMapEvent* Event, ZCompMapEventHandler* Handler)
{
    Event->handler = Handler;
}

void z_comp_mapevent_callHandler(const ZCompMapEvent* Event, ZWorld* World)
{
    Event->handler(a_component_getEntity(Event), World);
}
