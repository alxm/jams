#include <a2x.h>

#include "component_map.h"
#include "tiles.h"

struct ZCompMap {
    int w, h;
    ZTileType** tiles; // index to tiles
    AColMap* colmap;
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
    a_colmap_free(m->colmap);
}

void z_comp_map_init(ZCompMap* Map)
{
    Map->w = 8;
    Map->h = 8;
    Map->tiles = a_mem_malloc(Map->h * sizeof(ZTileType*));

    for(int i = Map->h; i--;) {
        Map->tiles[i] = a_mem_malloc(Map->w * sizeof(ZTileType));

        for(int j = Map->w; j--; ) {
            if(i == j) Map->tiles[i][j] = Z_TILE_TYPE_GROUND;
            else Map->tiles[i][j] = Z_TILE_TYPE_PEBBLES;

            if(a_random_int(5)==0 && i!=0 && j!=0) Map->tiles[i][j]=Z_TILE_TYPE_ROCK;
        }
    }

    Map->colmap = a_colmap_new(Map->w * Z_TILE_DIM,
                               Map->h * Z_TILE_DIM,
                               Z_TILE_DIM);
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

bool z_comp_map_canWalk(const ZCompMap* Map, int X, int Y)
{
    if(X < 0 || Y < 0) {
        return false;
    }

    X /= Z_TILE_DIM;
    Y /= Z_TILE_DIM;

    if(X >= Map->w || Y >= Map->h) {
        return false;
    }

    return z_tiles_isWalkable(Map->tiles[Y][X]);
}
