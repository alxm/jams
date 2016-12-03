#include <a2x.h>

#include "component_map.h"

void z_system_mapTick(AEntity* Entity, void* GlobalContext)
{
    //
}

void z_system_mapDraw(AEntity* Entity, void* GlobalContext)
{
    ZCompMap* map = a_entity_getComponent(Entity, "map");

    for(int y = 0; y < 16; y++) {
        for(int x = 0; x < 16; x++) {
            a_sprite_blit(z_comp_map_sprite(map, y, x),
                          x * 16,
                          y * 16);
        }
    }
}
