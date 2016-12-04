#include <a2x.h>

#include "component_input.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_velocity.h"
#include "controls.h"

static ZCompInputHandler z_entity_player_input;

AEntity* z_entity_player_new(void)
{
    AEntity* player = a_entity_new();

    z_comp_input_init(a_entity_addComponent(player, "input"), z_entity_player_input);
    z_comp_position_init(a_entity_addComponent(player, "position"), 0, 0);
    z_comp_sprite_init(a_entity_addComponent(player, "sprite"), "player");
    z_comp_velocity_init(a_entity_addComponent(player, "velocity"));

    return player;
}

static void z_entity_player_input(AEntity* Entity)
{
    ZCompPosition* position = a_entity_getComponent(Entity, "position");

    int xDir = 0;
    int yDir = 0;

    if(a_button_get(z_controls.up)) {
        yDir--;
    }

    if(a_button_get(z_controls.down)) {
        yDir++;
    }

    if(a_button_get(z_controls.left)) {
        xDir--;
    }

    if(a_button_get(z_controls.right)) {
        xDir++;
    }


    z_comp_position_move(position, xDir, yDir);
}
