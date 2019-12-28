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

#include "component_input.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_velocity.h"
#include "component_volume.h"
#include "controls.h"

static ZCompInputHandler z_entity_player_input;

AEntity* z_entity_player_new(int X, int Y, AColMap* Colmap)
{
    AEntity* player = a_entity_new();

    z_comp_input_init(a_entity_addComponent(player, "input"), z_entity_player_input);
    z_comp_position_init(a_entity_addComponent(player, "position"), X, Y);

    ZCompSprite* sprite = a_entity_addComponent(player, "sprite");
    z_comp_sprite_init(sprite, "playerUp playerDown playerLeft playerRight");
    z_comp_sprite_setDir(sprite, Z_COMP_SPRITE_DIRECTION_DOWN);

    z_comp_velocity_init(a_entity_addComponent(player, "velocity"));
    z_comp_volume_init(a_entity_addComponent(player, "volume"), Colmap, X, Y, 8, 8);

    return player;
}

static void z_entity_player_input(AEntity* Entity)
{
    ZCompPosition* position = a_entity_getComponent(Entity, "position");

    int xDir = 0;
    int yDir = 0;

    if(a_button_get(z_controls.up)) {
        yDir--;
    }

    if(a_button_get(z_controls.down)) {
        yDir++;
    }

    if(a_button_get(z_controls.left)) {
        xDir--;
    }

    if(a_button_get(z_controls.right)) {
        xDir++;
    }

    z_comp_position_setDir(position, xDir, yDir);
}
