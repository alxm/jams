#include <a2x.h>

#include "component_map.h"
#include "tiles.h"

struct ZCompMap {
    int w, h;
    int** tiles; // index to tiles
};

size_t z_comp_map_size(void)
{
    return sizeof(ZCompMap);
}

void z_comp_map_free(void* Self)
{
    ZCompMap* m = Self;

    for(int i = m->h; i--; ) {
        free(m->tiles[i]);
    }

    free(m->tiles);
}

void z_comp_map_init(ZCompMap* Map)
{
    Map->w = 32;
    Map->h = 32;
    Map->tiles = a_mem_malloc(Map->h * sizeof(int*));

    for(int i = Map->h; i--;) {
        Map->tiles[i] = a_mem_malloc(Map->w * sizeof(int));

        for(int j = Map->w; j--; ) {
            Map->tiles[i][j] = a_random_int(Z_TILE_TYPE_NUM);
        }
    }
}

ASprite* z_comp_map_sprite(const ZCompMap* Map, int X, int Y)
{
    return z_tiles_sprite(Map->tiles[Y][X]);
}

void z_comp_map_getDim(const ZCompMap* Map, int* W, int* H)
{
    *W = Map->w;
    *H = Map->h;
}
