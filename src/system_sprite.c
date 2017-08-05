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

#include "component_position.h"
#include "component_sprite.h"

void z_system_sprite(AEntity* Entity)
{
    ZCompPosition* position = a_entity_requireComponent(Entity, "position");
    ZCompSprite* sprite = a_entity_requireComponent(Entity, "sprite");

    AFix x, y;
    z_comp_position_getCoords(position, &x, &y);

    int originX, originY;
    z_state_game_getOrigin(a_entity_getContext(Entity), &originX, &originY);

    int scrOriginX = a_screen_getWidth() / 2;
    int scrOriginY = a_screen_getHeight() / 2;

    ASprite* graphic = z_comp_sprite_getGraphic(sprite);
    int gWidth = a_sprite_getWidth(graphic);
    int gHeight = a_sprite_getHeight(graphic);

    a_sprite_blit(graphic,
                  scrOriginX + a_fix_fixtoi(x) - originX - gWidth / 2,
                  scrOriginY + a_fix_fixtoi(y) - originY - gHeight / 2);
}
