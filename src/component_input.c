/*
    Copyright 2016 Alex Margarit

    This file is part of Motel 37.

    Motel 37 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Motel 37 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Motel 37.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_input.h"

struct ZCompInput {
    ZCompInputHandler* handler;
};

size_t z_comp_input_size(void)
{
    return sizeof(ZCompInput);
}

void z_comp_input_init(ZCompInput* Input, ZCompInputHandler* Handler)
{
    Input->handler = Handler;
}

void z_comp_input_callHandler(ZCompInput* Input)
{
    Input->handler(a_component_getEntity(Input));
}
