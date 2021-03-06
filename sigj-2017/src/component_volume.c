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

#include "component_volume.h"

struct ZCompVolume {
    int radius;
    AColObject* colobj;
};

size_t z_comp_volume_size(void)
{
    return sizeof(ZCompVolume);
}

void z_comp_volume_init(ZCompVolume* Volume, AColMap* ColMap, AFix X, AFix Y, int Radius)
{
    Volume->radius = Radius;
    Volume->colobj = a_colobject_new(ColMap, a_component_getEntity(Volume));

    a_colobject_setCoords(Volume->colobj, a_fix_fixtoi(X), a_fix_fixtoi(Y));
}

void z_comp_volume_free(void* Self)
{
    ZCompVolume* volume = Self;

    a_colobject_free(volume->colobj);
}

int z_comp_volume_getRadius(const ZCompVolume* Volume)
{
    return Volume->radius;
}

AColObject* z_comp_volume_getColObject(const ZCompVolume* Volume)
{
    return Volume->colobj;
}

void z_comp_volume_setCoords(const ZCompVolume* Volume, AFix X, AFix Y)
{
    a_colobject_setCoords(Volume->colobj, a_fix_fixtoi(X), a_fix_fixtoi(Y));
}
