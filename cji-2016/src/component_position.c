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

#include <a2x.h>

#include "component_position.h"

struct ZCompPosition {
    int x, y;
    int dirX, dirY;
    struct ZCompPosition* anchor;
};

size_t z_comp_position_size(void)
{
    return sizeof(ZCompPosition);
}

void z_comp_position_init(ZCompPosition* Position, int X, int Y)
{
    Position->x = X;
    Position->y = Y;
    Position->dirX = 0;
    Position->dirY = 0;
    Position->anchor = NULL;
}

void z_comp_position_initPointer(ZCompPosition* Position, ZCompPosition* Anchor)
{
    z_comp_position_init(Position, 0, 0);
    Position->anchor = Anchor;
}

void z_comp_position_setCoords(ZCompPosition* Position, int X, int Y)
{
    Position->x = X;
    Position->y = Y;
}

void z_comp_position_getCoords(const ZCompPosition* Position, int* X, int* Y)
{
    if(Position->anchor == NULL) {
        *X = Position->x;
        *Y = Position->y;
    } else {
        *X = Position->anchor->x;
        *Y = Position->anchor->y;
    }
}

void z_comp_position_setDir(ZCompPosition* Position, int DirX, int DirY)
{
    Position->dirX = DirX;
    Position->dirY = DirY;
}

void z_comp_position_retrieveDir(ZCompPosition* Position, int* DirX, int* DirY)
{
    *DirX = Position->dirX;
    *DirY = Position->dirY;

    Position->dirX = 0;
    Position->dirY = 0;
}
