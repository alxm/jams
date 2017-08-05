/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_input.h"

struct ZCompInput {
    AList* bindings;
};

struct ZCompInputBinding {
    AInputButton* button;
    ZCompInputHandler* handler;
};

size_t z_comp_input_size(void)
{
    return sizeof(ZCompInput);
}

void z_comp_input_init(ZCompInput* Input)
{
    Input->bindings = a_list_new();
}

void z_comp_input_free(void* Self)
{
    ZCompInput* input = Self;

    a_list_freeEx(input->bindings, free);
}

AList* z_comp_input_getBindings(const ZCompInput* Input)
{
    return Input->bindings;
}

AInputButton* z_comp_input_bindingGetButton(const ZCompInputBinding* Binding)
{
    return Binding->button;
}

ZCompInputHandler* z_comp_input_bindingGetHandler(const ZCompInputBinding* Binding)
{
    return Binding->handler;
}

void z_comp_input_addBinding(ZCompInput* Input, AInputButton* Button, ZCompInputHandler* Handler)
{
    ZCompInputBinding* b = a_mem_malloc(sizeof(ZCompInputBinding));

    b->button = Button;
    b->handler = Handler;

    a_list_addLast(Input->bindings, b);
}
