#include <a2x.h>

#include "component_position.h"
#include "component_sprite.h"

AEntity* z_entity_chest_new(int X, int Y)
{
    AEntity* chest = a_entity_new();

    z_comp_position_init(a_entity_addComponent(chest, "position"), X, Y);
    z_comp_sprite_init(a_entity_addComponent(chest, "sprite"), "chest");

    return chest;
}
