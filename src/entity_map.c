#include <a2x.h>

#include "component_map.h"

AEntity* z_entity_map_new(void)
{
    AEntity* map = a_entity_new();

    z_comp_map_init(a_entity_addComponent(map, "map"));

    return map;
}
