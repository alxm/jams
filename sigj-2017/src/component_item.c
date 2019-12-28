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

#include "component_item.h"

struct ZCompItem {
    char* name;
};

size_t z_comp_item_size(void)
{
    return sizeof(ZCompItem);
}

void z_comp_item_init(ZCompItem* Item, const char* Name)
{
    Item->name = a_str_dup(Name);
}

void z_comp_item_free(void* Self)
{
    ZCompItem* item = Self;

    free(item->name);
}

const char* z_comp_item_getName(const ZCompItem* Item)
{
    return Item->name;
}
