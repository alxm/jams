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

#include "component_health.h"

typedef struct {
    int startValue;
} CHealthData;

struct CHealth {
    int value;
};

static void c_health_dataInit(void* Data, const ABlock* Block, const void* Context)
{
    A_UNUSED(Context);

    CHealthData* data = Data;

    data->startValue = a_block_readInt(Block, 1);
}

static void c_health_initWithData(void* Self, const void* Data, const void* Context)
{
    A_UNUSED(Context);

    CHealth* health = Self;
    const CHealthData* data = Data;

    health->value = data->startValue;
}

void c_health_register(int Index)
{
    a_component_new(Index, "health", sizeof(CHealth), NULL, NULL);

    a_component_dataSet(Index,
                        sizeof(CHealthData),
                        c_health_dataInit,
                        NULL,
                        c_health_initWithData);
}

int c_health_valueGet(const CHealth* Health)
{
    return Health->value;
}

void c_health_valueSet(CHealth* Health, int Value)
{
    Health->value = Value;
}
