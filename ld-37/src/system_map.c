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

#include "tiles.h"
#include "component_map.h"
#include "component_position.h"
#include "world.h"

void z_system_mapDraw(AEntity* Entity, void* GlobalContext)
{
    AEntity* camera = z_world_getCamera(GlobalContext);
    ZCompPosition* position = a_entity_getComponent(camera, "position");
    ZCompMap* map = a_entity_getComponent(Entity, "map");

    int centerX, centerY;
    z_comp_position_getCoords(position, &centerX, &centerY);

    int mapW, mapH;
    z_comp_map_getDim(map, &mapW, &mapH);

    a_pixel_setRGB(0x11, 0x11, 0x11);
    a_draw_fill();

    const int startMapPixelX = centerX - a_screen_width() / 2;
    const int startMapPixelY = centerY - a_screen_height() / 2;
    int startDrawX = -(startMapPixelX % Z_TILE_DIM);
    int startDrawY = -(startMapPixelY % Z_TILE_DIM);
    int startMapTileX = startMapPixelX / Z_TILE_DIM;
    int startMapTileY = startMapPixelY / Z_TILE_DIM;
    const int endMapTileX = a_math_min(mapW, startMapTileX + a_screen_width() / Z_TILE_DIM + 1);
    const int endMapTileY = a_math_min(mapH, startMapTileY + a_screen_height() / Z_TILE_DIM + 1);

    if(startMapTileX < 0) {
        startDrawX += -startMapTileX * Z_TILE_DIM;
        startMapTileX = 0;
    }

    if(startMapTileY < 0) {
        startDrawY += -startMapTileY * Z_TILE_DIM;
        startMapTileY = 0;
    }

    for(int tileY = startMapTileY, drawY = startDrawY;
        tileY < endMapTileY;
        tileY++, drawY += Z_TILE_DIM) {

        for(int tileX = startMapTileX, drawX = startDrawX;
            tileX < endMapTileX;
            tileX++, drawX += Z_TILE_DIM) {

            a_sprite_blit(z_comp_map_sprite(map, tileX, tileY),
                          drawX,
                          drawY);
        }
    }
}
