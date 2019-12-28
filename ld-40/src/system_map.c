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
#include "util_level.h"
#include "util_terrain.h"

#include "component_mapgfx.h"
#include "component_mapterrain.h"

void z_system_mapFrame(AEntity* Entity)
{
    ZCompMapGfx* gfx = a_entity_reqComponent(Entity, "mapGfx");

    z_comp_mapgfx_tickFrame(gfx);
}

void z_system_mapDrawTiles(AEntity* Entity)
{
    ZCompMapGfx* gfx = a_entity_reqComponent(Entity, "mapGfx");
    ZCompMapTerrain* terrain = a_entity_reqComponent(Entity, "mapTerrain");

    int w, h;
    z_comp_mapterrain_getDim(terrain, &w, &h);

    for(int y = h; y--; ) {
        for(int x = w; x--; ) {
            ASprite* sprite = z_comp_mapgfx_getSprite(gfx, x, y);

            a_sprite_blit(sprite,
                          x * Z_UTIL_COORDS_TILE_DIM,
                          y * Z_UTIL_COORDS_TILE_DIM);
        }
    }
}
