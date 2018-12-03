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

#include "component_damage.h"

typedef struct {
    int startValue;
} CDamageData;

struct CDamage {
    int value;
};

static void c_damage_dataInit(void* Data, const ABlock* Block, const void* Context)
{
    A_UNUSED(Context);

    CDamageData* data = Data;

    data->startValue = a_block_readInt(Block, 1);
}

static void c_damage_initWithData(void* Self, const void* Data, const void* Context)
{
    A_UNUSED(Context);

    CDamage* damage = Self;
    const CDamageData* data = Data;

    damage->value = data->startValue;
}

void c_damage_register(int Index)
{
    a_component_new(Index, "damage", sizeof(CDamage), NULL, NULL);

    a_component_dataSet(Index,
                        sizeof(CDamageData),
                        c_damage_dataInit,
                        NULL,
                        c_damage_initWithData);
}

int c_damage_valueGet(const CDamage* Damage)
{
    return Damage->value;
}

void c_damage_valueSet(CDamage* Damage, int Value)
{
    Damage->value = Value;
}
