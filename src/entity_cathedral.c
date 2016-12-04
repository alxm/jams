#include <a2x.h>

#include "component_cathedral.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_volume.h"

AEntity* z_entity_cathedral_new(int X, int Y, AColMap* Colmap)
{
    AEntity* cathedral = a_entity_new();

    z_comp_cathedral_init(a_entity_addComponent(cathedral, "cathedral"));
    z_comp_position_init(a_entity_addComponent(cathedral, "position"), X, Y);
    z_comp_sprite_init(a_entity_addComponent(cathedral, "sprite"), "cathedral");
    z_comp_volume_init(a_entity_addComponent(cathedral, "volume"), 96, Colmap, X, Y);

    return cathedral;
}
