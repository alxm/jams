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

#include "component_position.h"
#include "component_velocity.h"

void z_system_move(AEntity* Entity)
{
    ZCompPosition* pos = a_entity_requireComponent(Entity, "position");
    ZCompVelocity* vel = a_entity_requireComponent(Entity, "velocity");

    AFix dx, dy;
    z_comp_velocity_getValues(vel, &dx, &dy);

    if(dx == 0 && dy == 0) {
        return;
    }

    AFix x, y;
    z_comp_position_getCoords(pos, &x, &y);

    z_comp_position_setCoords(pos, x + dx, y + dy);
    z_comp_velocity_setValues(vel, dx / 2, dy / 2);
}
