/*
    Copyright 2016 Alex Margarit

    This file is part of Church 2, a game prototype made for the
    Cool Jams Inc 48h game jam that ran from 2016-12-02 to 2016-12-04.

    Church 2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Church 2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Church 2.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef struct ZCompVolume ZCompVolume;

extern size_t z_comp_volume_size(void);
extern AComponentFree z_comp_volume_free;
extern void z_comp_volume_init(ZCompVolume* Volume, int Dim, AColMap* Colmap, int X, int Y);

extern int z_comp_volume_getDim(const ZCompVolume* Volume);
extern AColObject* z_comp_volume_getColobj(const ZCompVolume* Volume);
extern void z_comp_volume_setCoords(const ZCompVolume* Volume, int X, int Y);
