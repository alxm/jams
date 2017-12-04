/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef struct ZCompMapBuildings ZCompMapBuildings;

extern size_t z_comp_mapbuildings_size(void);
extern void z_comp_mapbuildings_init(ZCompMapBuildings* MapBuildings, const ZUtilLevel* Level);
extern AFree z_comp_mapbuildings_free;

extern AEntity* z_comp_mapbuildings_get(const ZCompMapBuildings* MapBuildings, int X, int Y);
extern void z_comp_mapbuildings_set(ZCompMapBuildings* MapBuildings, int X, int Y, AEntity* Building);
