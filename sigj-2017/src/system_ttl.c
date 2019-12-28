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

#include "component_ttl.h"

void z_system_ttl(AEntity* Entity)
{
    ZCompTtl* ttl = a_entity_requireComponent(Entity, "ttl");
    unsigned value = z_comp_ttl_getValue(ttl);

    if(value > 0) {
        z_comp_ttl_setValue(ttl, value - 1);
    } else {
        ZCompTtlCallback* callback = z_comp_ttl_getCallback(ttl);

        if(callback) {
            callback(Entity);
        }

        a_entity_remove(Entity);
    }
}
