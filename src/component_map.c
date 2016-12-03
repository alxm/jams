#include <a2x.h>

typedef struct ZCompMap {
    int w, h;
    int** tiles; // index to tiles
} ZCompMap;

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
