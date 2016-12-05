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

#include "component_cathedral.h"
#include "graphics.h"

struct ZCompCathedral {
    bool colorsOn;
    uint8_t angle;
};

size_t z_comp_cathedral_size(void)
{
    return sizeof(ZCompCathedral);
}

void z_comp_cathedral_init(ZCompCathedral* Cathedral)
{
    Cathedral->colorsOn = false;
    Cathedral->angle = 0;
}

ASprite* z_comp_cathedral_getColors(const ZCompCathedral* Cathedral)
{
    if(Cathedral->colorsOn) {
        return z_graphics_getStill("circleColors");
    } else {
        return z_graphics_getStill("circleB&W");
    }
}

void z_comp_cathedral_setColors(ZCompCathedral* Cathedral, bool ColorsOn)
{
    Cathedral->colorsOn = ColorsOn;
}

void z_comp_cathedral_incAngle(ZCompCathedral* Cathedral)
{
    Cathedral->angle++;
}

uint8_t z_comp_cathedral_getAngle(const ZCompCathedral* Cathedral)
{
    return Cathedral->angle;
}
