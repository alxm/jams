#include <a2x.h>

AEntity* z_entity_player_new(void)
{
    AEntity* player = a_entity_new();

    a_entity_addComponent(player, "position");
    a_entity_addComponent(player, "sprite");

    return player;
}
