#include <a2x.h>

#include "component_map.h"
#include "component_position.h"
#include "component_sprite.h"

#include "system_map.h"

#include "entity_camera.h"
#include "entity_map.h"
#include "entity_player.h"

typedef struct ZWorld {
    AEntity* map;
    AEntity* player;
    AEntity* camera;
} ZWorld;

A_STATE(world)
{
    static ZWorld world;

    A_STATE_INIT
    {
        a_component_declare("map", z_comp_map_size(), z_comp_map_free);
        a_component_declare("position", z_comp_position_size(), NULL);
        a_component_declare("sprite", z_comp_sprite_size(), z_comp_sprite_free);

        a_system_declare("mapTick", "map", z_system_mapTick);
        a_system_declare("mapDraw", "map", z_system_mapDraw);

        a_system_tick("mapTick");
        a_system_draw("mapDraw");

        a_system_setContext(&world);

        world.map = z_entity_map_new();
        world.player = z_entity_player_new();
        world.camera = z_entity_camera_new(world.player);
    }

    A_STATE_BODY
    {
        A_STATE_LOOP;
    }

    A_STATE_FREE
    {
        a_entity_free(world.camera);
        a_entity_free(world.map);
        a_entity_free(world.player);
    }
}

AEntity* z_world_getCamera(const ZWorld* World)
{
    return World->camera;
}
