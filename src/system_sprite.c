#include <a2x.h>

#include "component_position.h"
#include "component_sprite.h"

#include "world.h"

void z_system_sprite(AEntity* Entity, void* GlobalContext)
{
    AEntity* camera = z_world_getCamera(GlobalContext);
    ZCompPosition* cameraPosition = a_entity_getComponent(camera, "position");
    ZCompPosition* spritePosition = a_entity_getComponent(Entity, "position");
    ZCompSprite* sprite = a_entity_getComponent(Entity, "sprite");

    int camX, camY;
    z_comp_position_getCoords(cameraPosition, &camX, &camY);

    int sprX, sprY;
    z_comp_position_getCoords(spritePosition, &sprX, &sprY);

    a_sprite_blit(z_comp_sprite_getFrame(sprite),
                  a_screen_width() / 2 + (sprX - camX),
                  a_screen_height() / 2 + (sprY - camY));
}
