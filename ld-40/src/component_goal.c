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

typedef struct {
    ZCompGoalState state;
    int destX, destY;
    int nextX, nextY;
    AEntity* objective;
} ZGoalData;

struct ZCompGoal {
    ZGoalData data;
    AList* stack; // list of ZGoalData
};

size_t z_comp_goal_size(void)
{
    return sizeof(ZCompGoal);
}

void z_comp_goal_init(ZCompGoal* Goal)
{
    Goal->stack = a_list_new();

    z_comp_goal_clearNextCoords(Goal);
}

void z_comp_goal_free(void* Self)
{
    ZCompGoal* goal = Self;

    if(goal->data.objective != NULL) {
        a_entity_release(goal->data.objective);
    }

    A_LIST_ITERATE(goal->stack, ZGoalData*, entry) {
        if(entry->objective != NULL) {
            a_entity_release(entry->objective);
        }

        free(entry);
    }

    a_list_free(goal->stack);

}

ZCompGoalState z_comp_goal_getState(const ZCompGoal* Goal)
{
    return Goal->data.state;
}

void z_comp_goal_setState(ZCompGoal* Goal, ZCompGoalState State)
{
    Goal->data.state = State;
}

void z_comp_goal_getDestCoords(const ZCompGoal* Goal, int* DestX, int* DestY)
{
    *DestX = Goal->data.destX;
    *DestY = Goal->data.destY;
}

void z_comp_goal_setDestCoords(ZCompGoal* Goal, int DestX, int DestY)
{
    Goal->data.destX = DestX;
    Goal->data.destY = DestY;
}

void z_comp_goal_getNextCoords(const ZCompGoal* Goal, int* NextX, int* NextY)
{
    *NextX = Goal->data.nextX;
    *NextY = Goal->data.nextY;
}

void z_comp_goal_setNextCoords(ZCompGoal* Goal, int NextX, int NextY)
{
    Goal->data.nextX = NextX;
    Goal->data.nextY = NextY;
}

void z_comp_goal_clearNextCoords(ZCompGoal* Goal)
{
    Goal->data.nextX = -1;
    Goal->data.nextY = -1;
}

bool z_comp_goal_hasNextCoords(const ZCompGoal* Goal)
{
    return Goal->data.nextX != -1 && Goal->data.nextY != -1;
}

AEntity* z_comp_goal_getObjective(const ZCompGoal* Goal)
{
    return Goal->data.objective;
}

void z_comp_goal_setObjective(ZCompGoal* Goal, AEntity* Objective)
{
    if(Goal->data.objective != NULL) {
        a_entity_release(Goal->data.objective);
    }

    Goal->data.objective = Objective;

    if(Objective != NULL) {
        a_entity_reference(Objective);
    }
}

void z_comp_goal_push(ZCompGoal* Goal)
{
    a_list_push(Goal->stack, a_mem_dup(&Goal->data, sizeof(ZGoalData)));

    Goal->data.objective = NULL;
}

bool z_comp_goal_pop(ZCompGoal* Goal)
{
    if(a_list_isEmpty(Goal->stack)) {
        return false;
    }

    if(Goal->data.objective != NULL) {
        a_entity_release(Goal->data.objective);
    }

    ZGoalData* d = a_list_pop(Goal->stack);
    Goal->data = *d;
    free(d);

    return true;
}
