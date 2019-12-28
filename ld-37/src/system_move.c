/*
    Copyright 2016 Alex Margarit

    This file is part of Motel 37.

    Motel 37 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Motel 37 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Motel 37.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "controls.h"
#include "tiles.h"
#include "world.h"
#include "component_map.h"
#include "component_mapevent.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_velocity.h"
#include "component_volume.h"

void z_system_move(AEntity* Entity, void* GlobalContext)
{
    ZCompPosition* position = a_entity_getComponent(Entity, "position");
    ZCompSprite* sprite = a_entity_getComponent(Entity, "sprite");

    int xDir, yDir;
    z_comp_position_retrieveDir(position, &xDir, &yDir);

    if(xDir == 0 && yDir == 0) {
        if(sprite) {
            z_comp_sprite_stop(sprite);
        }

        return;
    }

    ZCompVelocity* velocity = a_entity_getComponent(Entity, "velocity");
    ZCompVolume* volume = a_entity_getComponent(Entity, "volume");
    ZCompMap* map = a_entity_getComponent(z_world_getMap(GlobalContext), "map");

    int oldX, oldY;
    z_comp_position_getCoords(position, &oldX, &oldY);

    int speed = z_comp_velocity_getSpeed(velocity);
    int newX = oldX + xDir * speed;
    int newY = oldY + yDir * speed;

    int x1, y1;

    if(z_comp_map_canWalk(map, newX, newY)
        && z_comp_map_canWalk(map, newX, oldY)
        && z_comp_map_canWalk(map, oldX, newY)) {
        x1 = newX;
        y1 = newY;
    } else if(z_comp_map_canWalk(map, newX, oldY)) {
        x1 = newX;
        y1 = oldY;
    } else if(z_comp_map_canWalk(map, oldX, newY)) {
        x1 = oldX;
        y1 = newY;
    } else {
        x1 = oldX;
        y1 = oldY;
    }

    z_comp_position_setCoords(position, x1, y1);

    if(volume != NULL) {
        int width1, height1;
        z_comp_volume_getDim(volume, &width1, &height1);
        z_comp_volume_setCoords(volume, x1, y1);

        A_COL_ITERATE(z_comp_volume_getColobj(volume), ZCompVolume*, v) {
            AEntity* entity2 = a_component_getEntity(v);
            ZCompPosition* pos2 = a_entity_getComponent(entity2, "position");

            int x2, y2;
            z_comp_position_getCoords(pos2, &x2, &y2);

            int width2, height2;
            z_comp_volume_getDim(v, &width2, &height2);

            if(a_collide_boxes(x1 - width1 / 2, y1 - height1 / 2, width1, height1,
                               x2 - width2 / 2, y2 - height2 / 2, width2, height2)) {

                z_comp_position_setCoords(position, oldX, oldY);
                z_comp_volume_setCoords(volume, oldX, oldY);

                ZCompMapEvent* event = a_entity_getComponent(entity2, "mapevent");

                if(event != NULL) {
                    z_controls_unpress();
                    z_comp_mapevent_callHandler(event, GlobalContext);
                }

                break;
            }
        }
    }

    if(sprite) {
        z_comp_sprite_move(sprite);

        if(newY > oldY) {
            z_comp_sprite_setDir(sprite, Z_COMP_SPRITE_DIRECTION_DOWN);
        } else if(newY < oldY) {
            z_comp_sprite_setDir(sprite, Z_COMP_SPRITE_DIRECTION_UP);
        } else if(newX > oldX) {
            z_comp_sprite_setDir(sprite, Z_COMP_SPRITE_DIRECTION_RIGHT);
        } else if(newX < oldX) {
            z_comp_sprite_setDir(sprite, Z_COMP_SPRITE_DIRECTION_LEFT);
        }
    }
}
