#include <a2x.h>

#include "component_input.h"
#include "component_map.h"
#include "component_mapevent.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_velocity.h"
#include "component_volume.h"

#include "system_input.h"
#include "system_map.h"
#include "system_move.h"
#include "system_sprite.h"

#include "entity_camera.h"
#include "entity_chest.h"
#include "entity_map.h"
#include "entity_player.h"

#include "tiles.h"

typedef struct ZWorld {
    AEntity* map;
    AColMap* colmap;
    AEntity* player;
    AEntity* camera;
} ZWorld;

A_STATE(world)
{
    static ZWorld world;

    A_STATE_INIT
    {
        a_component_declare("input", z_comp_input_size(), NULL);
        a_component_declare("map", z_comp_map_size(), z_comp_map_free);
        a_component_declare("mapevent", z_comp_mapevent_size(), NULL);
        a_component_declare("position", z_comp_position_size(), NULL);
        a_component_declare("sprite", z_comp_sprite_size(), z_comp_sprite_free);
        a_component_declare("velocity", z_comp_velocity_size(), NULL);
        a_component_declare("volume", z_comp_volume_size(), NULL);

        a_system_declare("input", "input", z_system_input);
        a_system_declare("mapDraw", "map", z_system_mapDraw);
        a_system_declare("move", "position velocity", z_system_move);
        a_system_declare("sprite", "position sprite", z_system_sprite);

        a_system_tick("input move");
        a_system_draw("mapDraw sprite");

        a_system_setContext(&world);

        world.map = z_entity_map_new();

        int mapW, mapH;
        z_comp_map_getDim(a_entity_getComponent(world.map, "map"), &mapW, &mapH);
        world.colmap = a_colmap_new(mapW * Z_TILE_DIM,
                                    mapH * Z_TILE_DIM,
                                    Z_TILE_DIM);

        world.player = z_entity_player_new(0, 0, world.colmap);
        world.camera = z_entity_camera_new(world.player);

        z_entity_chest_new(40, 40, world.colmap);
    }

    A_STATE_BODY
    {
        A_STATE_LOOP;
    }

    A_STATE_FREE
    {
        a_entity_free(world.camera);
        a_entity_free(world.player);
        a_colmap_free(world.colmap);
        a_entity_free(world.map);
    }
}

AEntity* z_world_getMap(const ZWorld* World)
{
    return World->map;
}

AEntity* z_world_getCamera(const ZWorld* World)
{
    return World->camera;
}
