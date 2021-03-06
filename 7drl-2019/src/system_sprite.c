#include "system_sprite.h"

#include "component_position.h"
#include "component_sprite.h"
#include "obj_camera.h"
#include "util_ecs.h"

void s_sprite_tick(AEntity* Entity)
{
    CSprite* sprite = a_entity_componentReq(Entity, U_COM_SPRITE);

    c_sprite_tick(sprite);
}

void s_sprite_draw(AEntity* Entity)
{
    CPosition* position = a_entity_componentReq(Entity, U_COM_POSITION);
    CSprite* sprite = a_entity_componentReq(Entity, U_COM_SPRITE);

    AVectorInt tile = c_position_coordsGet(position);
    AVectorFix world = a_vectorint_toFix(tile);
    AVectorInt screen = n_camera_coordsToScreen(world);

    c_sprite_draw(sprite, screen);
}
