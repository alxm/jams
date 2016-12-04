#include <a2x.h>

#include "component_map.h"
#include "component_position.h"

#include "tiles.h"
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
    const int endMapTileX = a_math_min(mapW - 1, startMapTileX + a_screen_width() / Z_TILE_DIM + 1);
    const int endMapTileY = a_math_min(mapH - 1, startMapTileY + a_screen_height() / Z_TILE_DIM + 1);

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
