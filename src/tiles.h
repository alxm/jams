#define Z_TILE_DIM 16

typedef enum ZTileType {
    Z_TILE_TYPE_GROUND,
    Z_TILE_TYPE_PEBBLES,
    Z_TILE_TYPE_ROCK,
    Z_TILE_TYPE_PEBBLES2,
    Z_TILE_TYPE_ROCK2,
    Z_TILE_TYPE_WEEDS,
    Z_TILE_TYPE_NUM
} ZTileType;

extern void z_tiles_load(void);

extern ASprite* z_tiles_sprite(size_t Index);
extern bool z_tiles_isWalkable(size_t Index);
