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

struct ZCompGoal {
    ZCompGoalState state;
    int destX, destY;
    int nextX, nextY;
    AEntity* objective;
};

size_t z_comp_goal_size(void)
{
    return sizeof(ZCompGoal);
}

void z_comp_goal_init(ZCompGoal* Goal)
{
    z_comp_goal_clearNextCoords(Goal);
}

void z_comp_goal_free(void* Self)
{
    ZCompGoal* goal = Self;

    if(goal->objective != NULL) {
        a_entity_release(goal->objective);
    }
}

ZCompGoalState z_comp_goal_getState(const ZCompGoal* Goal)
{
    return Goal->state;
}

void z_comp_goal_setState(ZCompGoal* Goal, ZCompGoalState State)
{
    Goal->state = State;
}

void z_comp_goal_getDestCoords(const ZCompGoal* Goal, int* DestX, int* DestY)
{
    *DestX = Goal->destX;
    *DestY = Goal->destY;
}

void z_comp_goal_setDestCoords(ZCompGoal* Goal, int DestX, int DestY)
{
    Goal->destX = DestX;
    Goal->destY = DestY;
}

void z_comp_goal_getNextCoords(const ZCompGoal* Goal, int* NextX, int* NextY)
{
    *NextX = Goal->nextX;
    *NextY = Goal->nextY;
}

void z_comp_goal_setNextCoords(ZCompGoal* Goal, int NextX, int NextY)
{
    Goal->nextX = NextX;
    Goal->nextY = NextY;
}

void z_comp_goal_clearNextCoords(ZCompGoal* Goal)
{
    Goal->nextX = -1;
    Goal->nextY = -1;
}

bool z_comp_goal_hasNextCoords(const ZCompGoal* Goal)
{
    return Goal->nextX != -1 && Goal->nextY != -1;
}

AEntity* z_comp_goal_getObjective(const ZCompGoal* Goal)
{
    return Goal->objective;
}

void z_comp_goal_setObjective(ZCompGoal* Goal, AEntity* Objective)
{
    if(Goal->objective != NULL) {
        a_entity_release(Goal->objective);
    }

    Goal->objective = Objective;

    if(Objective != NULL) {
        a_entity_reference(Objective);
    }
}
