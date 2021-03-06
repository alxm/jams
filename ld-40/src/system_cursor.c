#include <a2x.h>

#include "state_game.h"

#include "util_colors.h"
#include "util_controls.h"
#include "util_coords.h"
#include "util_frames.h"
#include "util_level.h"
#include "util_terrain.h"

#include "component_cursor.h"
#include "component_goal.h"
#include "component_mapterrain.h"
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

static void handlePosition(ZCompCursor* Cursor, ZCompPosition* Position, ZCompVolume* Volume)
{
    int cursorX, cursorY;
    z_comp_position_getCoordsInt(Position, &cursorX, &cursorY);

    int cursorRadius = z_comp_volume_getRadius(Volume);

    z_comp_cursor_setHover(Cursor, NULL);

    A_COL_ITERATE(z_comp_volume_getColObject(Volume), AEntity*, unit) {
        ZCompPosition* position = a_entity_reqComponent(unit, "position");
        ZCompVolume* volume = a_entity_reqComponent(unit, "volume");

        int unitX, unitY;
        z_comp_position_getCoordsInt(position, &unitX, &unitY);

        int unitRadius = z_comp_volume_getRadius(volume);

        if(a_collide_circleAndCircle(cursorX,
                                     cursorY,
                                     cursorRadius,
                                     unitX,
                                     unitY,
                                     unitRadius)) {

            z_comp_cursor_setHover(Cursor, unit);
            break;
        }
    }
}

static void dispatchMission(ZStateGame* Game, ZCompCursor* Cursor, ZCompPosition* Position)
{
    AEntity* hoverUnit = z_comp_cursor_getHover(Cursor);
    AEntity* selectedUnit = z_comp_cursor_getSelected(Cursor);

    int cursorX, cursorY;
    z_comp_position_getCoordsInt(Position, &cursorX, &cursorY);

    if(hoverUnit != NULL) {
        ZCompVolume* volume = a_entity_reqComponent(hoverUnit, "volume");

        if(z_comp_volume_isSelectable(volume)) {
            z_comp_cursor_setSelected(Cursor, hoverUnit);
        } else if(selectedUnit != NULL) {
            ZCompGoal* goal = a_entity_getComponent(selectedUnit, "goal");

            if(goal) {
                z_comp_goal_setDestCoords(goal, cursorX, cursorY);
                z_comp_goal_setObjective(goal, hoverUnit);
                z_comp_goal_setState(goal, Z_COMP_GOAL_STATE_PATHFINDING);
            }
        }
    } else if(selectedUnit != NULL) {
        int cursorTileX = z_util_coords_intToTileInt(cursorX);
        int cursorTileY = z_util_coords_intToTileInt(cursorY);

        AEntity* map = z_state_game_getMap(Game);
        ZCompMapTerrain* mapTerrain = a_entity_reqComponent(map, "mapTerrain");
        ZUtilTerrainType terrain = z_comp_mapterrain_getType(mapTerrain,
                                                             cursorTileX,
                                                             cursorTileY);

        if(z_util_terrain_isWalkable(terrain)) {
            ZCompGoal* goal = a_entity_getComponent(selectedUnit, "goal");

            if(goal) {
                z_comp_goal_setDestCoords(goal, cursorX, cursorY);
                z_comp_goal_setObjective(goal, hoverUnit);
                z_comp_goal_setState(goal, Z_COMP_GOAL_STATE_PATHFINDING);
            }
        } else {
            z_comp_cursor_setSelected(Cursor, NULL);
        }
    }
}

void z_system_cursorTick(AEntity* Entity)
{
    ZCompCursor* cursor = a_entity_reqComponent(Entity, "cursor");
    ZCompPosition* position = a_entity_reqComponent(Entity, "position");
    ZCompVolume* volume = a_entity_reqComponent(Entity, "volume");

    updateMouseCoords(position, volume);
    handlePosition(cursor, position, volume);

    if(a_touch_getTap(z_util_controls.mouse)) {
        dispatchMission(a_entity_getContext(Entity), cursor, position);
    }

    z_comp_cursor_lightTick(cursor);
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
