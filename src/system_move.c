#include <a2x.h>

#include "component_map.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_velocity.h"

#include "world.h"

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
    ZCompMap* map = a_entity_getComponent(z_world_getMap(GlobalContext), "map");

    int oldX, oldY;
    z_comp_position_getCoords(position, &oldX, &oldY);

    int speed = z_comp_velocity_getSpeed(velocity);
    int newX = oldX + xDir * speed;
    int newY = oldY + yDir * speed;

    if(z_comp_map_canWalk(map, newX, newY)) {
        z_comp_position_setCoords(position, newX, newY);
    } else if(z_comp_map_canWalk(map, newX, oldY)) {
        z_comp_position_setCoords(position, newX, oldY);
    } else if(z_comp_map_canWalk(map, oldX, newY)) {
        z_comp_position_setCoords(position, oldX, newY);
    }

    if(sprite) {
        z_comp_sprite_move(sprite);
    }
}
