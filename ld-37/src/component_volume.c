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

#include "component_volume.h"

struct ZCompVolume {
    int width, height;
    AColObject* colobject;
};

size_t z_comp_volume_size(void)
{
    return sizeof(ZCompVolume);
}

void z_comp_volume_free(void* Self)
{
    ZCompVolume* volume = Self;
    a_colobject_free(volume->colobject);
}

void z_comp_volume_init(ZCompVolume* Volume, AColMap* Colmap, int X, int Y, int Width, int Height)
{
    Volume->width = Width;
    Volume->height = Height;
    Volume->colobject = a_colobject_new(Colmap, Volume);
    a_colobject_setCoords(Volume->colobject, X, Y);
}

void z_comp_volume_getDim(const ZCompVolume* Volume, int* Width, int* Height)
{
    *Width = Volume->width;
    *Height = Volume->height;
}

AColObject* z_comp_volume_getColobj(const ZCompVolume* Volume)
{
    return Volume->colobject;
}

void z_comp_volume_setCoords(const ZCompVolume* Volume, int X, int Y)
{
    a_colobject_setCoords(Volume->colobject, X, Y);
}
