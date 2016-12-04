#include <a2x.h>

#include "component_position.h"
#include "component_velocity.h"

#include "world.h"

void z_system_move(AEntity* Entity, void* GlobalContext)
{
    ZCompPosition* position = a_entity_getComponent(Entity, "position");
    ZCompVelocity* velocity = a_entity_getComponent(Entity, "velocity");
//    ZCompMap map = a_entity_getComponent(z_world_getMap(GlobalContext), "map");

    z_comp_position_doMove(position, z_comp_velocity_getSpeed(velocity));
}
