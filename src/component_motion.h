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

typedef struct ZCompMotion ZCompMotion;

typedef enum {
    Z_COMP_MOTION_STATE_OK,
    Z_COMP_MOTION_STATE_BLOCKED,
} ZCompMotionState;

typedef enum {
    Z_COMP_MOTION_DIR_UP,
    Z_COMP_MOTION_DIR_DOWN,
    Z_COMP_MOTION_DIR_LEFT,
    Z_COMP_MOTION_DIR_RIGHT
} ZCompMotionDirection;

extern size_t z_comp_motion_size(void);

extern ZCompMotionState z_comp_motion_getState(const ZCompMotion* Motion);
extern void z_comp_motion_setState(ZCompMotion* Motion, ZCompMotionState State);

extern ZCompMotionDirection z_comp_motion_getDirection(const ZCompMotion* Motion);
extern void z_comp_motion_setDirection(ZCompMotion* Motion, ZCompMotionDirection Direction);
