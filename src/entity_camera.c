#include <a2x.h>

#include "component_position.h"

AEntity* z_entity_camera_new(AEntity* Anchor)
{
    AEntity* camera = a_entity_new();

    ZCompPosition* position = a_entity_getComponent(Anchor, "position");

    z_comp_position_initPointer(a_entity_addComponent(camera, "position"),
                                position);

    return camera;
}
