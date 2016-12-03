#include <a2x.h>

#include "component_map.h"
#include "component_position.h"

#include "tiles.h"
#include "world.h"

void z_system_mapTick(AEntity* Entity, void* GlobalContext)
{
    //
}

void z_system_mapDraw(AEntity* Entity, void* GlobalContext)
{
    AEntity* camera = z_world_getCamera(GlobalContext);
    ZCompPosition* position = a_entity_getComponent(camera, "position");
    ZCompMap* map = a_entity_getComponent(Entity, "map");

    int centerX, centerY;
    z_comp_position_getCoords(position, &centerX, &centerY);

    int mapW, mapH;
    z_comp_map_getDim(map, &mapW, &mapH);

    int startTileX = a_math_max(0, (centerX - a_screen_width() / 2) / Z_TILE_DIM);
    int startTileY = a_math_max(0, (centerY - a_screen_height() / 2) / Z_TILE_DIM);
    int endTileX = a_math_min(mapW - 1, startTileX + a_screen_width() / Z_TILE_DIM);
    int endTileY = a_math_min(mapH - 1, startTileY + a_screen_height() / Z_TILE_DIM);
    int startDrawX = -(centerX % Z_TILE_DIM);
    int startDrawY = -(centerY % Z_TILE_DIM);

    for(int tileY = startTileY; tileY < endTileY; tileY++) {
        for(int tileX = startTileX; tileX < endTileX; tileX++) {
            a_sprite_blit(z_comp_map_sprite(map, tileX, tileY),
                          startDrawX + (tileX - startTileX) * Z_TILE_DIM,
                          startDrawY + (tileY - startTileY) * Z_TILE_DIM);
        }
    }
}
