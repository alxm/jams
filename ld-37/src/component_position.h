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

typedef struct ZCompPosition ZCompPosition;

extern size_t z_comp_position_size(void);
extern void z_comp_position_init(ZCompPosition* Position, int X, int Y);
extern void z_comp_position_initPointer(ZCompPosition* Position, ZCompPosition* Anchor);

extern void z_comp_position_setCoords(ZCompPosition* Position, int X, int Y);
extern void z_comp_position_getCoords(const ZCompPosition* Position, int* X, int* Y);
extern void z_comp_position_setDir(ZCompPosition* Position, int DirX, int DirY);
extern void z_comp_position_retrieveDir(ZCompPosition* Position, int* DirX, int* DirY);
