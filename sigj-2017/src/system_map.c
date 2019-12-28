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

#include "util_colors.h"
#include "util_tiles.h"

#include "component_map.h"
#include "component_position.h"

void z_system_mapDraw(AEntity* Entity)
{
    a_pixel_setPixel(z_util_colors.gray1);
    a_draw_fill();

    int screenWidth = a_screen_getWidth();
    int screenHeight = a_screen_getHeight();

    ZCompMap* map = a_entity_requireComponent(Entity, "map");

    int mapWidth, mapHeight;
    z_comp_map_getDim(map, &mapWidth, &mapHeight);

    int originX, originY;
    z_state_game_getOrigin(a_entity_getContext(Entity), &originX, &originY);

    int startX = originX - screenWidth / 2;
    int startY = originY - screenHeight / 2;

    int startTileX = startX >> Z_UTIL_TILE_SHIFT;
    int startTileY = startY >> Z_UTIL_TILE_SHIFT;

    int offsetX = startX & Z_UTIL_TILE_MASK;
    int offsetY = startY & Z_UTIL_TILE_MASK;

    int startDrawX = -offsetX;
    int startDrawY = -offsetY;

    if(startTileX < 0) {
        startDrawX += -startTileX * Z_UTIL_TILE_DIM;
        startTileX = 0;
    }

    if(startTileY < 0) {
        startDrawY += -startTileY * Z_UTIL_TILE_DIM;
        startTileY = 0;
    }

    int endTileX = a_math_min(mapWidth,
                              startTileX + (screenWidth - startDrawX) / Z_UTIL_TILE_DIM + 1);
    int endTileY = a_math_min(mapHeight,
                              startTileY + (screenHeight - startDrawY) / Z_UTIL_TILE_DIM + 1);

    for(int tileY = startTileY, drawY = startDrawY;
        tileY < endTileY;
        tileY++, drawY += Z_UTIL_TILE_DIM) {

        for(int tileX = startTileX, drawX = startDrawX;
            tileX < endTileX;
            tileX++, drawX += Z_UTIL_TILE_DIM) {

            if(tileX >= 0 && tileY >= 0) {
                ASprite* tile = z_comp_map_getSprite(map, tileX, tileY);
                a_sprite_blit(tile, drawX, drawY);
            }
        }
    }
}
