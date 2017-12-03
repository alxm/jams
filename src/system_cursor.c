#include <a2x.h>

#include "util_controls.h"

#include "component_position.h"

void z_system_cursorTick(AEntity* Entity)
{
    ZCompPosition* position = a_entity_reqComponent(Entity, "position");

    AFix x, y;
    z_comp_position_getCoordsFix(position, &x, &y);

    int mouseDx, mouseDy;
    a_touch_getDelta(z_util_controls.mouse, &mouseDx, &mouseDy);

    x = a_math_clamp(x + a_fix_itofix(mouseDx),
                     0,
                     a_fix_itofix(a_screen_getWidth()) - 1);
    y = a_math_clamp(y + a_fix_itofix(mouseDy),
                     0,
                     a_fix_itofix(a_screen_getHeight()) - 1);

    z_comp_position_setCoords(position, x, y);
}
