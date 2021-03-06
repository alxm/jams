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

typedef struct ZCompMapTerrain ZCompMapTerrain;

extern size_t z_comp_mapterrain_size(void);
extern void z_comp_mapterrain_init(ZCompMapTerrain* MapTerrain, const ZUtilLevel* Level);
extern AFree z_comp_mapterrain_free;

extern const ZUtilTerrainType** z_comp_mapterrain_getMap(const ZCompMapTerrain* MapTerrain);
extern void z_comp_mapterrain_getDim(const ZCompMapTerrain* MapTerrain, int* W, int* H);
extern ZUtilTerrainType z_comp_mapterrain_getType(const ZCompMapTerrain* MapTerrain, int X, int Y);
