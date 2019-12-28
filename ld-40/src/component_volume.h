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

typedef struct ZCompVolume ZCompVolume;

extern size_t z_comp_volume_size(void);
extern void z_comp_volume_init(ZCompVolume* Volume, AColMap* ColMap, int X, int Y, int Radius, bool IsObstacle, bool IsSelectable);
extern AFree z_comp_volume_free;

extern int z_comp_volume_getRadius(const ZCompVolume* Volume);
extern AColObject* z_comp_volume_getColObject(const ZCompVolume* Volume);
extern void z_comp_volume_setCoords(const ZCompVolume* Volume, AFix X, AFix Y);
extern bool z_comp_volume_isObstacle(const ZCompVolume* Volume);
extern bool z_comp_volume_isSelectable(const ZCompVolume* Volume);
