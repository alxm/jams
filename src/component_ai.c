/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Cave Shrine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cave Shrine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cave Shrine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "component_ai.h"

struct CAi {
    int value;
};

static void c_ai_init(void* Self)
{
    CAi* ai = Self;

    ai->value = 1;
}

static void c_ai_free(void* Self)
{
    CAi* ai = Self;

    A_UNUSED(ai);
}

void c_ai_register(int Index)
{
    a_component_new(Index, "ai", sizeof(CAi), c_ai_init, c_ai_free);
}

int c_ai_valueGet(const CAi* Ai)
{
    return Ai->value;
}

void c_ai_valueSet(CAi* Ai, int Value)
{
    Ai->value = Value;
}
