#include <a2x.h>

#include "component_position.h"
#include "component_sprite.h"

AEntity* z_entity_player_new(void)
{
    AEntity* player = a_entity_new();

    z_comp_position_init(a_entity_addComponent(player, "position"), 0, 0);
    z_comp_sprite_init(a_entity_addComponent(player, "sprite"), "player");

    return player;
}
