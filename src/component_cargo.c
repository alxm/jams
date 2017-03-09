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

#include "component_cargo.h"

struct ZCompCargo {
    int contents[Z_CARGO_TYPE_NUM];
};

size_t z_comp_cargo_size(void)
{
    return sizeof(ZCompCargo);
}

void z_comp_cargo_init(ZCompCargo* Cargo)
{
    memset(Cargo->contents, 0, sizeof(Cargo->contents));
}

int z_comp_cargo_getContent(const ZCompCargo* Cargo, ZCargoType Type)
{
    return Cargo->contents[Type];
}

void z_comp_cargo_addContent(ZCompCargo* Cargo, ZCargoType Type, int Num)
{
    Cargo->contents[Type] += Num;
}

const char* z_comp_cargo_getName(ZCargoType Type, bool Plural)
{
    static const char* names[Z_CARGO_TYPE_NUM][2] = {
        {"cred", "creds"},
        {"fuel unit", "fuel units"},
    };

    return names[Type][Plural];
}
