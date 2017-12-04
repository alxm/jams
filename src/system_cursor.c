#include <a2x.h>

#include "util_colors.h"
#include "util_controls.h"
#include "util_coords.h"
#include "util_frames.h"

#include "component_cursor.h"
#include "component_position.h"
#include "component_volume.h"

void z_system_cursorTick(AEntity* Entity)
{
    ZCompCursor* cursor = a_entity_reqComponent(Entity, "cursor");
    ZCompPosition* position = a_entity_reqComponent(Entity, "position");
    ZCompVolume* volume = a_entity_reqComponent(Entity, "volume");

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
    z_comp_volume_setCoords(volume, x, y);

    int cursorX, cursorY;
    z_comp_position_getCoordsInt(position, &cursorX, &cursorY);

    int cursorRadius = z_comp_volume_getRadius(volume);

    bool hover = false;

    A_COL_ITERATE(z_comp_volume_getColObject(volume), AEntity*, worker) {
        ZCompPosition* position = a_entity_reqComponent(worker, "position");
        ZCompVolume* volume = a_entity_reqComponent(worker, "volume");

        int workerX, workerY;
        z_comp_position_getCoordsInt(position, &workerX, &workerY);

        int workerRadius = z_comp_volume_getRadius(volume);

        if(a_collide_circleAndCircle(cursorX,
                                     cursorY,
                                     cursorRadius,
                                     workerX,
                                     workerY,
                                     workerRadius)) {

            hover = true;
            z_comp_cursor_setHover(cursor, worker);

            if(a_touch_getTap(z_util_controls.mouse)) {
                z_comp_cursor_setSelected(cursor, worker);
            }

            break;
        }
    }

    if(!hover) {
        z_comp_cursor_setHover(cursor, NULL);

        if(a_touch_getTap(z_util_controls.mouse)) {
            z_comp_cursor_setSelected(cursor, NULL);
        }
    }

    z_comp_cursor_lightTick(cursor);
}

static void drawUnderlight(const ZCompCursor* Cursor, ZCompCursorType Type, AEntity* Unit)
{
    ZCompPosition* position = a_entity_reqComponent(Unit, "position");

    int unitX, unitY;
    z_comp_position_getCoordsInt(position, &unitX, &unitY);

    ASprite* sprite = z_comp_cursor_getLight(Cursor, Type);

    a_sprite_blit(sprite,
                  unitX - a_sprite_getWidth(sprite) / 2,
                  unitY - a_sprite_getHeight(sprite) / 2);
}

void z_system_cursorDrawUnderside(AEntity* Entity)
{
    ZCompCursor* cursor = a_entity_reqComponent(Entity, "cursor");
    ZCompPosition* position = a_entity_reqComponent(Entity, "position");

    int x, y;
    z_comp_position_getCoordsInt(position, &x, &y);

    AEntity* hoverUnit = z_comp_cursor_getHover(cursor);
    AEntity* selectedUnit = z_comp_cursor_getSelected(cursor);

    if(hoverUnit != NULL && hoverUnit != selectedUnit) {
        drawUnderlight(cursor, Z_COMP_CURSOR_UNIT_HOVER, hoverUnit);
    }

    if(selectedUnit != NULL) {
        drawUnderlight(cursor, Z_COMP_CURSOR_UNIT_SELECTED, selectedUnit);
    }
}
