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

#include "component_input.h"

struct CInput {
    AList* bindings; // list of CInputBinding
};

typedef struct {
    UButtonId button;
    CInputHandler* handler;
} CInputBinding;

static void c_input_init(void* Self)
{
    CInput* input = Self;

    input->bindings = a_list_new();
}

static void c_input_free(void* Self)
{
    CInput* input = Self;

    a_list_freeEx(input->bindings, free);
}

void c_input_register(int Index)
{
    a_component_new(Index, "input", sizeof(CInput), c_input_init, c_input_free);
}

static CInputBinding* bindingNew(UButtonId Button, CInputHandler* Handler)
{
    CInputBinding* b = a_mem_malloc(sizeof(CInputBinding));

    b->button = Button;
    b->handler = Handler;

    return b;
}

void c_input_bind(const CInput* Input, UButtonId Button, CInputHandler* Handler)
{
    a_list_addLast(Input->bindings, bindingNew(Button, Handler));
}

void c_input_tick(const CInput* Input)
{
    A_LIST_ITERATE(Input->bindings, CInputBinding*, b) {
        if(a_button_pressGet(u_controls_get(b->button))) {
            b->handler(a_component_entityGet(Input), b->button);
        }
    }
}
