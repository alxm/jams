/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
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

void z_comp_input_runHandler(const ZCompInput* Input)
{
    Input->handler(a_component_getEntity(Input));
}
