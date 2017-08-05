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

typedef struct ZCompMap ZCompMap;

extern size_t z_comp_map_size(void);
extern void z_comp_map_init(ZCompMap* Map, ASprite* Data);
extern AComponentFree z_comp_map_free;

extern void z_comp_map_getDim(const ZCompMap* Map, int* Width, int* Height);
extern bool z_comp_map_isWalkable(const ZCompMap* Map, int X, int Y);
