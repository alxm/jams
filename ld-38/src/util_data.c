/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    38th Century - A simple world simulation game made for Ludum Dare 38 72h

    38th Century is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    38th Century is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 38th Century.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_data.h"

typedef struct ZEntry {
    int values[2];
    int ceiling;
} ZEntry;

struct ZData {
    ZEntry entries[Z_DATA_NUM];
};

typedef bool (*ZStatsUpdate)(int Values[2]);

static const char* g_names[Z_DATA_NUM];

static bool updateValueNow(int Values[2]) {
    Values[1] = Values[0];
    return true;
}

static bool updateValueSmooth(int Values[2]) {
    bool finished;
    int diff = a_math_abs(Values[0] - Values[1]);

    if(diff == 0 || diff > 1000) {
        Values[1] = Values[0];
        finished = true;
    } else {
        int inc = Values[0] > Values[1] ? 1 : (Values[0] < Values[1] ? -1 : 0);

        if(diff > 100) {
            Values[1] += 100 * inc;
            finished = false;
        } else if(diff > 10) {
            Values[1] += 10 * inc;
            finished = false;
        } else {
            Values[1] += inc;
            finished = false;
        }

        if((inc > 0 && Values[1] > Values[0]) || (inc < 0 && Values[1] < Values[0])) {
            Values[1] = Values[0];
            finished = true;
        }
    }

    return finished;
}

void z_data_load(void)
{
    g_names[Z_DATA_YEAR] = "YEAR";
    g_names[Z_DATA_POPULATION] = "POPULATION";
    g_names[Z_DATA_DEFENSE] = "DEFENSE";
    g_names[Z_DATA_HEALTH] = "HEALTH";
    g_names[Z_DATA_INDUSTRY] = "INDUSTRY";
    g_names[Z_DATA_SCIENCE] = "SCIENCE";
}

ZData* z_data_new(void)
{
    ZData* d = a_mem_zalloc(sizeof(ZData));

    for(ZDataType t = 0; t < Z_DATA_NUM; t++) {
        d->entries[t].ceiling = 256;
    }

    d->entries[Z_DATA_YEAR].ceiling = INT_MAX;
    d->entries[Z_DATA_POPULATION].ceiling = a_fix_itofix(0xffff);

    return d;
}

void z_data_free(ZData* Data)
{
    free(Data);
}

int z_data_getReal(const ZData* Data, ZDataType Type)
{
    return Data->entries[Type].values[0];
}

int z_data_getShow(const ZData* Data, ZDataType Type)
{
    return Data->entries[Type].values[1];
}

void z_data_set(ZData* Data, ZDataType Type, int Value)
{
    Value = a_math_constrain(Value, 0, Data->entries[Type].ceiling);
    Data->entries[Type].values[0] = Value;
}

void z_data_inc(ZData* Data, ZDataType Type, int Inc)
{
    z_data_set(Data, Type, z_data_getReal(Data, Type) + Inc);
}

bool z_data_isMin(ZData* Data, ZDataType Type)
{
    return Data->entries[Type].values[0] <= 0;
}

bool z_data_isMax(ZData* Data, ZDataType Type)
{
    return Data->entries[Type].values[0] >= Data->entries[Type].ceiling;
}

const char* z_data_getName(ZDataType Type)
{
    return g_names[Type];
}

bool z_data_tick(ZData* Data, bool Smooth)
{
    bool finished = true;
    ZStatsUpdate update = Smooth ? updateValueSmooth : updateValueNow;

    for(ZDataType d = 0; d < Z_DATA_NUM; d++) {
        finished &= update(Data->entries[d].values);
    }

    return finished;
}
