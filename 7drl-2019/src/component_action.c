/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of City, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "component_action.h"

struct CAction {
    int value;
};

const size_t c_action_size = sizeof(CAction);

void c_action_init(void* Self)
{
    CAction* action = Self;

    action->value = 1;
}

void c_action_free(void* Self)
{
    CAction* action = Self;

    A_UNUSED(action);
}

int c_action_valueGet(const CAction* Action)
{
    return Action->value;
}

void c_action_valueSet(CAction* Action, int Value)
{
    Action->value = Value;
}
