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

#include "component_bag.h"

struct ZCompBag {
    AList* items; // list of AEntity
};

size_t z_comp_bag_size(void)
{
    return sizeof(ZCompBag);
}

void z_comp_bag_init(ZCompBag* Bag)
{
    Bag->items = a_list_new();
}

void z_comp_bag_free(void* Self)
{
    ZCompBag* bag = Self;

    a_list_free(bag->items);
}

void z_comp_bag_add(ZCompBag* Bag, AEntity* Entity)
{
    a_list_addLast(Bag->items, Entity);
}
