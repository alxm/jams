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

#include "component_map.h"

void z_system_mapDraw(AEntity* Entity)
{
    ZCompMap* map = a_entity_requireComponent(Entity, "map");

    for(int i = 0; i < Z_MAP_TILES_H; i++) {
        for(int j = 0; j < Z_MAP_TILES_W; j++) {
            ASprite* tileSprite = z_comp_map_getTileSprite(map, j, i);
            a_sprite_blit(tileSprite, j * Z_MAP_TILE_DIM, i * Z_MAP_TILE_DIM);
        }
    }
}
