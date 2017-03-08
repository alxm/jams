/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_graphics.h"

#include "component_health.h"
#include "component_position.h"
#include "component_sprite.h"

#include "state_game.h"

void z_system_sprite(AEntity* Entity)
{
    ZCompSprite* sprite = a_entity_requireComponent(Entity, "sprite");
    ZCompPosition* position = a_entity_requireComponent(Entity, "position");
    ZCompHealth* health = a_entity_getComponent(Entity, "health");

    int x, y;
    z_comp_position_getCoords(position, &x, &y);

    a_sprite_blit(z_comp_sprite_getFrame(sprite),
                  x * Z_MAP_TILE_DIM,
                  y * Z_MAP_TILE_DIM);

    if(health && Entity != z_game_getPlayer()) {
        int points, max;
        z_comp_health_getStats(health, &points, &max);

        int totalWidth = Z_MAP_TILE_DIM - 4;
        int greenWidth = totalWidth * points / max;
        int redWidth = totalWidth - greenWidth;

        a_pixel_setHex(0x00bb00);
        a_draw_rectangle(x * Z_MAP_TILE_DIM + 2,
                         y * Z_MAP_TILE_DIM,
                         greenWidth,
                         2);

        a_pixel_setHex(0xbb0000);
        a_draw_rectangle(x * Z_MAP_TILE_DIM + 2 + greenWidth,
                         y * Z_MAP_TILE_DIM,
                         redWidth,
                         2);
    }
}
