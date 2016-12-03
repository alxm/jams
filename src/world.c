#include <a2x.h>

#include "component_map.h"
#include "component_position.h"
#include "component_sprite.h"

#include "system_map.h"

#include "entity_map.h"
#include "entity_player.h"

typedef struct ZGame {
    AEntity* map;
    AEntity* player;
} ZGame;

A_STATE(world)
{
    static ZGame game;

    A_STATE_INIT
    {
        a_component_declare("map", z_comp_map_size(), z_comp_map_free);
        a_component_declare("position", z_comp_position_size(), NULL);
        a_component_declare("sprite", z_comp_sprite_size(), z_comp_sprite_free);

        a_system_declare("mapTick", "map", z_system_mapTick);
        a_system_declare("mapDraw", "map", z_system_mapDraw);

        a_system_tick("mapTick");
        a_system_draw("mapDraw");

        a_system_setContext(&game);

        game.map = z_entity_map_new();
        game.player = z_entity_player_new();
    }

    A_STATE_BODY
    {
        A_STATE_LOOP;
    }

    A_STATE_FREE
    {
        a_entity_free(game.map);
        a_entity_free(game.player);
    }
}
