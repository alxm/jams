#include <a2x.h>

#include "component_position.h"
#include "component_sprite.h"
#include "component_volume.h"

AEntity* z_entity_cathedral_new(int X, int Y, AColMap* Colmap)
{
    AEntity* chest = a_entity_new();

    z_comp_position_init(a_entity_addComponent(chest, "position"), X, Y);
    z_comp_volume_init(a_entity_addComponent(chest, "volume"), 96, Colmap, X, Y);
    z_comp_sprite_init(a_entity_addComponent(chest, "sprite"), "cathedral");

    return chest;
}
