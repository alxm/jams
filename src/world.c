#include <a2x.h>

#include "map.h"
#include "player.h"

typedef struct ZGame {
    AEntity* map;
    AEntity* player;
} ZGame;

A_STATE(world)
{
    static ZGame game;

    A_STATE_INIT
    {
        a_component_declare("map", z_map_comp_size(), z_map_comp_free);
        a_component_declare("position", z_comp_position_size(), NULL);

        a_system_declare("mapTick", "map", z_map_comp_handler_tick);
        a_system_declare("mapDraw", "map", z_map_comp_handler_draw);

        a_system_draw("mapDraw");

        a_system_setContext(&game);

        game.map = z_map_new();
    }

    A_STATE_BODY
    {
        printf("whahaa\n");

        A_STATE_LOOP
        {
            //
        }
    }

    A_STATE_FREE
    {
        a_entity_free(game.map);
    }
}
