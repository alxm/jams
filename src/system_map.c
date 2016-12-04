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

    int startMapPixelX = centerX - a_screen_width() / 2;
    int startMapPixelY = centerY - a_screen_height() / 2;
    int startMapTileX = startMapPixelX / Z_TILE_DIM;
    int startMapTileY = startMapPixelY / Z_TILE_DIM;
    int xOffset = startMapPixelX % Z_TILE_DIM;
    int yOffset = startMapPixelY % Z_TILE_DIM;

    for(int drawY = -yOffset, tileY = startMapTileY; drawY < a_screen_height(); drawY += Z_TILE_DIM, tileY++) {
        for(int drawX = -xOffset, tileX = startMapTileX; drawX < a_screen_width(); drawX += Z_TILE_DIM, tileX++) {
            if(tileX >= 0 && tileX < mapW && tileY >= 0 && tileY < mapH) {
                a_sprite_blit(z_comp_map_sprite(map, tileX, tileY),
                              drawX,
                              drawY);
            }
        }
    }
}
