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

typedef struct ZCompGoal ZCompGoal;

typedef enum {
    Z_COMP_GOAL_STATE_INVALID = -1,
    Z_COMP_GOAL_STATE_NONE,
    Z_COMP_GOAL_STATE_MOVE,
} ZCompGoalState;

extern size_t z_comp_goal_size(void);
extern void z_comp_goal_init(ZCompGoal* Goal);
extern AFree z_comp_goal_free;

extern ZCompGoalState z_comp_goal_getState(const ZCompGoal* Goal);
extern void z_comp_goal_setState(ZCompGoal* Goal, ZCompGoalState State);

extern void z_comp_goal_getDestCoords(const ZCompGoal* Goal, int* DestX, int* DestY);
extern void z_comp_goal_setDestCoords(ZCompGoal* Goal, int DestX, int DestY);

extern void z_comp_goal_getNextCoords(const ZCompGoal* Goal, int* NextX, int* NextY);
extern void z_comp_goal_setNextCoords(ZCompGoal* Goal, int NextX, int NextY);

extern AEntity* z_comp_goal_getObjective(const ZCompGoal* Goal);
extern void z_comp_goal_setObjective(ZCompGoal* Goal, AEntity* Objective);
