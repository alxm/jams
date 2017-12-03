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

#include "component_position.h"
#include "component_sprite.h"

int z_system_sprite_sort(AEntity* A, AEntity* B)
{
    ZCompSprite* spriteA = a_entity_reqComponent(A, "sprite");
    ZCompSprite* spriteB = a_entity_reqComponent(B, "sprite");

    return z_comp_sprite_getLayer(spriteA) - z_comp_sprite_getLayer(spriteB);
}

void z_system_spriteTickFrame(AEntity* Entity)
{
    z_comp_sprite_tickFrame(a_entity_reqComponent(Entity, "sprite"));
}

void z_system_spriteDraw(AEntity* Entity)
{
    ZCompPosition* position = a_entity_reqComponent(Entity, "position");
    ZCompSprite* sprite = a_entity_reqComponent(Entity, "sprite");

    int x, y;
    z_comp_position_getCoordsInt(position, &x, &y);

    ASprite* spr = z_comp_sprite_getSprite(sprite);
    int w = a_sprite_getWidth(spr);
    int h = a_sprite_getHeight(spr);

    a_sprite_blit(spr, x - w / 2, y - h / 2);
}
