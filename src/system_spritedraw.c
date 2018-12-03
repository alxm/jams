#include "system_spritedraw.h"

#include "component_position.h"
#include "component_sprite.h"

#include "util_ecs.h"
#include "util_map.h"

static void s_spritedraw(AEntity* Entity)
{
    CPosition* cposition = a_entity_componentReq(Entity, U_COM_POSITION);
    CSprite* csprite = a_entity_componentReq(Entity, U_COM_SPRITE);

    AVectorInt coords = c_position_coordsGet(cposition);
    CPositionDirection dir = c_position_directionGet(cposition);

    a_sprite_blit(c_sprite_framesGet(csprite, dir),
                  coords.x * U_TILE_DIM,
                  coords.y * U_TILE_DIM);
}

void s_spritedraw_register(int Index)
{
    a_system_new(Index, s_spritedraw, NULL, false);
    a_system_add(Index, U_COM_POSITION);
    a_system_add(Index, U_COM_SPRITE);
}
