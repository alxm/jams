#include <a2x.h>

#include "component_cathedral.h"
#include "component_position.h"
#include "world.h"

void z_system_cathedralTick(AEntity* Entity, void* GlobalContext)
{
    //
}

void z_system_cathedralDraw(AEntity* Entity, void* GlobalContext)
{
    AEntity* camera = z_world_getCamera(GlobalContext);
    ZCompPosition* cameraPosition = a_entity_getComponent(camera, "position");
    ZCompPosition* spritePosition = a_entity_getComponent(Entity, "position");
    ZCompCathedral* cathedral = a_entity_getComponent(Entity, "cathedral");
    ASprite* s = z_comp_cathedral_getGlass(cathedral);

    int camX, camY;
    z_comp_position_getCoords(cameraPosition, &camX, &camY);

    int sprX, sprY;
    z_comp_position_getCoords(spritePosition, &sprX, &sprY);

    a_sprite_blit(s,
                  a_screen_width() / 2 + (sprX - camX) - a_sprite_w(s) / 2,
                  a_screen_height() / 2 + (sprY - camY));
}
