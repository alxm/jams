/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "state_game.h"

#include "util_coords.h"
#include "util_level.h"
#include "util_terrain.h"

#include "component_mapterrain.h"
#include "component_motion.h"
#include "component_position.h"
#include "component_velocity.h"
#include "component_volume.h"

void z_system_move(AEntity* Entity)
{
    ZCompMotion* motion = a_entity_reqComponent(Entity, "motion");
    ZCompPosition* position = a_entity_reqComponent(Entity, "position");
    ZCompVelocity* velocity = a_entity_reqComponent(Entity, "velocity");
    ZCompVolume* volume = a_entity_reqComponent(Entity, "volume");

    AFix dx, dy;
    z_comp_velocity_getDelta(velocity, &dx, &dy);

    if(dx == 0 && dy == 0) {
        z_comp_motion_setState(motion, Z_COMP_MOTION_STATE_RESTING);
        return;
    }

    z_comp_velocity_setDelta(velocity, 0, 0);

    AFix x, y;
    z_comp_position_getCoordsFix(position, &x, &y);

    AFix oldX = x;
    AFix oldY = y;
    AFix newX = x + dx;
    AFix newY = y + dy;

    ZStateGame* game = a_entity_getContext(Entity);
    AEntity* map = z_state_game_getMap(game);
    ZCompMapTerrain* terrainMap = a_entity_reqComponent(map, "mapTerrain");

    int tileX = z_util_coords_fixToTileInt(newX);
    int tileY = z_util_coords_fixToTileInt(newY);
    ZUtilTerrainType terrain = z_comp_mapterrain_getType(terrainMap,
                                                         tileX,
                                                         tileY);

    if(!z_util_terrain_isWalkable(terrain)) {
        z_comp_motion_setState(motion, Z_COMP_MOTION_STATE_BLOCKED);
        return;
    }

    int x1 = a_fix_fixtoi(newX);
    int y1 = a_fix_fixtoi(newY);
    int r1 = z_comp_volume_getRadius(volume);

    z_comp_volume_setCoords(volume, newX, newY);

    A_COL_ITERATE(z_comp_volume_getColObject(volume), AEntity*, e) {
        if(a_entity_isMuted(e)) {
            continue;
        }

        ZCompPosition* ePosition = a_entity_reqComponent(e, "position");
        ZCompVolume* eVolume = a_entity_reqComponent(e, "volume");

        if(!z_comp_volume_isObstacle(eVolume)) {
            continue;
        }

        int x2, y2;
        z_comp_position_getCoordsInt(ePosition, &x2, &y2);

        int r2 = z_comp_volume_getRadius(eVolume);

        if(a_collide_circleAndCircle(x1, y1, r1, x2, y2, r2)) {
            z_comp_volume_setCoords(volume, oldX, oldY);
            z_comp_motion_setState(motion, Z_COMP_MOTION_STATE_BLOCKED);

            a_entity_sendMessage(Entity, e, "collided");

            return;
        }
    }

    z_comp_position_setCoords(position, newX, newY);
    z_comp_motion_setState(motion, Z_COMP_MOTION_STATE_MOVING);
}
