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

typedef struct {
    int code;
} CAiData;

struct CAi {
    int code;
};

static void c_ai_dataInit(void* Data, const ABlock* Block, const void* Context)
{
    A_UNUSED(Context);

    CAiData* data = Data;

    data->code = a_block_readInt(Block, 1);
}

static void c_ai_initWithData(void* Self, const void* Data, const void* Context)
{
    A_UNUSED(Context);

    CAi* ai = Self;
    const CAiData* data = Data;

    ai->code = data->code;
}

void c_ai_register(int Index)
{
    a_component_new(Index, "ai", sizeof(CAi), NULL, NULL);

    a_component_dataSet(Index,
                        sizeof(CAiData),
                        c_ai_dataInit,
                        NULL,
                        c_ai_initWithData);
}

int c_ai_codeGet(const CAi* Ai)
{
    return Ai->code;
}
