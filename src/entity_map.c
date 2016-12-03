#include <a2x.h>

AEntity* z_entity_map_new(void)
{
    AEntity* map = a_entity_new();

    a_entity_addComponent(map, "map");

    return map;
}
