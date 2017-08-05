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

#include "state_game.h"

#include "util_controls.h"
#include "util_tiles.h"

#include "component_input.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_velocity.h"

static void move(AEntity* Entity, AFix Ddx, AFix Ddy)
{
    ZCompVelocity* velocity = a_entity_requireComponent(Entity, "velocity");
    ZCompSprite* sprite = a_entity_requireComponent(Entity, "sprite");

    AFix dx, dy;
    z_comp_velocity_getValues(velocity, &dx, &dy);

    dx = a_math_constrain(dx + Ddx, -A_FIX_ONE, A_FIX_ONE);
    dy = a_math_constrain(dy + Ddy, -A_FIX_ONE, A_FIX_ONE);

    z_comp_velocity_setValues(velocity, dx, dy);

    if(Ddy < 0) {
        z_comp_sprite_setDirection(sprite, Z_COMP_SPRITE_UP);
    } else if(Ddy > 0) {
        z_comp_sprite_setDirection(sprite, Z_COMP_SPRITE_DOWN);
    } else if(Ddx < 0) {
        z_comp_sprite_setDirection(sprite, Z_COMP_SPRITE_LEFT);
    } else if(Ddx > 0) {
        z_comp_sprite_setDirection(sprite, Z_COMP_SPRITE_RIGHT);
    }
}

static void moveUp(AEntity* Entity)
{
    move(Entity, 0, -A_FIX_ONE);
}

static void moveDown(AEntity* Entity)
{
    move(Entity, 0, +A_FIX_ONE);
}

static void moveLeft(AEntity* Entity)
{
    move(Entity, -A_FIX_ONE, 0);
}

static void moveRight(AEntity* Entity)
{
    move(Entity, +A_FIX_ONE, 0);
}

AEntity* z_entity_player_new(ZStateGame* Game, int TileX, int TileY)
{
    AEntity* e = a_entity_new("player", Game);

    ZCompInput* input = a_entity_addComponent(e, "input");
    z_comp_input_init(input);
    z_comp_input_addBinding(input, z_util_controls.up, moveUp);
    z_comp_input_addBinding(input, z_util_controls.down, moveDown);
    z_comp_input_addBinding(input, z_util_controls.left, moveLeft);
    z_comp_input_addBinding(input, z_util_controls.right, moveRight);

    ZCompPosition* position = a_entity_addComponent(e, "position");
    z_comp_position_init(position,
                         a_fix_itofix(TileX * Z_UTIL_TILE_DIM
                                        + Z_UTIL_TILE_DIM / 2),
                         a_fix_itofix(TileY * Z_UTIL_TILE_DIM
                                        + Z_UTIL_TILE_DIM / 2));

    ZCompSprite* sprite = a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite,
                       "playerUp",
                       "playerDown",
                       "playerLeft",
                       "playerRight");

    a_entity_addComponent(e, "velocity");

    return e;
}
