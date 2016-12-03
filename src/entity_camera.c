#include <a2x.h>

#include "component_position.h"

AEntity* z_entity_camera_new(void)
{
    AEntity* camera = a_entity_new();

    z_comp_position_init(a_entity_addComponent(camera, "position"), 0, 0);

    return camera;
}
