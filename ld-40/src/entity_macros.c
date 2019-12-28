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

#include <a2x.h>

#include "util_coords.h"

#include "component_motion.h"
#include "component_sprite.h"
#include "component_velocity.h"

static void move(AEntity* Entity, AFix Ddx, AFix Ddy, ZUtilDirection Direction)
{
    ZCompMotion* motion = a_entity_reqComponent(Entity, "motion");
    ZCompSprite* sprite = a_entity_reqComponent(Entity, "sprite");
    ZCompVelocity* velocity = a_entity_reqComponent(Entity, "velocity");

    AFix dx, dy;
    z_comp_velocity_getDelta(velocity, &dx, &dy);

    dx = a_math_clamp(dx + Ddx, -A_FIX_ONE, A_FIX_ONE);
    dy = a_math_clamp(dy + Ddy, -A_FIX_ONE, A_FIX_ONE);

    z_comp_velocity_setDelta(velocity, dx, dy);

    z_comp_motion_setDirection(motion, Direction);
    z_comp_sprite_setDirection(sprite, Direction);
}

void z_entity_macro_moveUp(AEntity* Entity)
{
    move(Entity, 0, -A_FIX_ONE, Z_UTIL_DIRECTION_UP);
}

void z_entity_macro_moveDown(AEntity* Entity)
{
    move(Entity, 0, +A_FIX_ONE, Z_UTIL_DIRECTION_DOWN);
}

void z_entity_macro_moveLeft(AEntity* Entity)
{
    move(Entity, -A_FIX_ONE, 0, Z_UTIL_DIRECTION_LEFT);
}

void z_entity_macro_moveRight(AEntity* Entity)
{
    move(Entity, +A_FIX_ONE, 0, Z_UTIL_DIRECTION_RIGHT);
}
