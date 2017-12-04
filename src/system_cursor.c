#include <a2x.h>

#include "util_colors.h"
#include "util_controls.h"
#include "util_coords.h"
#include "util_frames.h"

#include "component_cursor.h"
#include "component_goal.h"
#include "component_position.h"
#include "component_volume.h"

static void updateMouseCoords(ZCompPosition* Position, ZCompVolume* Volume)
{
    AFix x, y;
    z_comp_position_getCoordsFix(Position, &x, &y);

    int mouseDx, mouseDy;
    a_touch_getDelta(z_util_controls.mouse, &mouseDx, &mouseDy);

    x = a_math_clamp(x + a_fix_itofix(mouseDx),
                     0,
                     a_fix_itofix(a_screen_getWidth()) - 1);
    y = a_math_clamp(y + a_fix_itofix(mouseDy),
                     0,
                     a_fix_itofix(a_screen_getHeight()) - 1);

    z_comp_position_setCoords(Position, x, y);
    z_comp_volume_setCoords(Volume, x, y);
}

static void handleClick(ZCompCursor* Cursor, ZCompPosition* Position, ZCompVolume* Volume)
{
    int cursorX, cursorY;
    z_comp_position_getCoordsInt(Position, &cursorX, &cursorY);

    int cursorRadius = z_comp_volume_getRadius(Volume);

    bool hover = false;

    A_COL_ITERATE(z_comp_volume_getColObject(Volume), AEntity*, worker) {
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
            z_comp_cursor_setHover(Cursor, worker);

            if(a_touch_getTap(z_util_controls.mouse)) {
                z_comp_cursor_setSelected(Cursor, worker);
            }

            break;
        }
    }

    if(!hover) {
        z_comp_cursor_setHover(Cursor, NULL);

        if(a_touch_getTap(z_util_controls.mouse)) {
            z_comp_cursor_setSelected(Cursor, NULL);
        }
    }
}

static void dispatchMission(ZCompCursor* Cursor, ZCompPosition* Position, AEntity* Actor, AEntity* Objective)
{
    if(Objective == NULL) {
        ZCompGoal* goal = a_entity_getComponent(Actor, "goal");

        if(goal == NULL) {
            return;
        }

        int x, y;
        z_comp_position_getCoordsInt(Position, &x, &y);

        z_comp_goal_setDestCoords(goal, x, y);
        z_comp_goal_setObjective(goal, Objective);
        z_comp_goal_setState(goal, Z_COMP_GOAL_STATE_PATHFINDING);
    } else if(Actor != Objective) {
        A_UNUSED(Cursor);
        //z_comp_cursor_setSelected(Cursor, Actor);
    }
}

void z_system_cursorTick(AEntity* Entity)
{
    ZCompCursor* cursor = a_entity_reqComponent(Entity, "cursor");
    ZCompPosition* position = a_entity_reqComponent(Entity, "position");
    ZCompVolume* volume = a_entity_reqComponent(Entity, "volume");

    AEntity* oldSelected = z_comp_cursor_getSelected(cursor);

    updateMouseCoords(position, volume);
    handleClick(cursor, position, volume);
    z_comp_cursor_lightTick(cursor);

    if(oldSelected != NULL) {
        AEntity* newSelected = z_comp_cursor_getSelected(cursor);
        dispatchMission(cursor, position, oldSelected, newSelected);
    }
}

static void drawUnderlight(const ZCompCursor* Cursor, ZCompCursorType Type, AEntity* Unit)
{
    ZCompPosition* position = a_entity_reqComponent(Unit, "position");

    int unitX, unitY;
    z_comp_position_getCoordsInt(position, &unitX, &unitY);

    ZCompCursorSize size = Z_COMP_CURSOR_SIZE_SMALL;

    if(a_entity_hasComponent(Unit, "tagBuilding")) {
        size = Z_COMP_CURSOR_SIZE_LARGE;
    }

    ASprite* sprite = z_comp_cursor_getLight(Cursor, Type, size);

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
        drawUnderlight(cursor, Z_COMP_CURSOR_TYPE_HOVER, hoverUnit);
    }

    if(selectedUnit != NULL) {
        drawUnderlight(cursor, Z_COMP_CURSOR_TYPE_SELECTED, selectedUnit);
    }
}
