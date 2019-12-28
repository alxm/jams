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

#include "component_motion.h"

struct ZCompMotion {
    ZCompMotionState state;
    ZCompMotionDirection direction;
};

size_t z_comp_motion_size(void)
{
    return sizeof(ZCompMotion);
}

ZCompMotionState z_comp_motion_getState(const ZCompMotion* Motion)
{
    return Motion->state;
}

void z_comp_motion_setState(ZCompMotion* Motion, ZCompMotionState State)
{
    Motion->state = State;
}

ZCompMotionDirection z_comp_motion_getDirection(const ZCompMotion* Motion)
{
    return Motion->direction;
}

void z_comp_motion_setDirection(ZCompMotion* Motion, ZCompMotionDirection Direction)
{
    Motion->direction = Direction;
}
