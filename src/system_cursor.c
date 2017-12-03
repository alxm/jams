#include <a2x.h>

#include "util_colors.h"
#include "util_controls.h"
#include "util_coords.h"

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


void z_system_cursorDrawUnderside(AEntity* Entity)
{
    ZCompPosition* position = a_entity_reqComponent(Entity, "position");

    int x, y;
    z_comp_position_getCoordsInt(position, &x, &y);

    int tileX = z_util_coords_intToTileInt(x);
    int tileY = z_util_coords_intToTileInt(y);

    a_pixel_push();
    a_pixel_setBlend(A_PIXEL_BLEND_ADD);
    a_pixel_setPixel(z_util_colors_get(Z_UTIL_COLOR_PURPLE, 0));

    a_draw_rectangle(tileX * Z_UTIL_COORDS_TILE_DIM,
                     tileY * Z_UTIL_COORDS_TILE_DIM,
                     Z_UTIL_COORDS_TILE_DIM,
                     Z_UTIL_COORDS_TILE_DIM);

    a_pixel_pop();
}
