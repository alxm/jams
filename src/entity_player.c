#include <a2x.h>

typedef struct ZPlayer {
    AFix x, y; // pixel-level coords on world map
    ASpriteFrames* frames;
} ZPlayer;

size_t z_player_comp_size(void)
{
    return sizeof(ZPlayer);
}

AEntity* z_player_new(void)
{
    AEntity* entity = a_entity_new();

    return entity;
}

void z_player_comp_free(void* Self)
{
    ZMap* m = Self;

    for(int i = m->h; i--; ) {
        free(m->tiles[i]);
    }

    free(m->tiles);
    free(m);
}

void z_player_comp_handler_tick(AEntity* Entity, void* GlobalContext)
{
    //
}

void z_player_comp_handler_draw(AEntity* Entity, void* GlobalContext)
{
    //
}
