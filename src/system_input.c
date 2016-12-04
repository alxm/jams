#include <a2x.h>

#include "component_input.h"

void z_system_input(AEntity* Entity, void* GlobalContext)
{
    z_comp_input_callHandler(a_entity_getComponent(Entity, "input"));
}
