/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_goal.h"
#include "component_position.h"

void z_system_goalTick(AEntity* Entity)
{
    ZCompGoal* goal = a_entity_reqComponent(Entity, "goal");
    ZCompPosition* position = a_entity_reqComponent(Entity, "position");

    ZCompGoalState state = z_comp_goal_getState(goal);

    if(state == Z_COMP_GOAL_STATE_NONE) {
        return;
    }

    int x, y;
    z_comp_position_getCoordsInt(position, &x, &y);

    int destX, destY;
    z_comp_goal_getDestCoords(goal, &destX, &destY);

    // TODO: path find from current to destination, set next tile target
}
