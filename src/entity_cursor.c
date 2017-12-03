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

#include "state_game.h"

#include "component_cursor.h"
#include "component_position.h"
#include "component_sprite.h"

AEntity* z_entity_cursor_new(ZStateGame* Game)
{
    AEntity* e = a_entity_new("cursor", Game);

    ZCompCursor* cursor = a_entity_addComponent(e, "cursor");
    z_comp_cursor_init(cursor);

    ZCompPosition* position = a_entity_addComponent(e, "position");
    z_comp_position_init(position,
                         a_fix_itofix(a_screen_getWidth() / 2),
                         a_fix_itofix(a_screen_getHeight() / 2));

    ZCompSprite* sprite =  a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite, "cursor", Z_COMP_SPRITE_LAYER_CURSOR);

    return e;
}
