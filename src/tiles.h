#define Z_TILE_DIM 16

typedef enum ZTileType {
    Z_TILE_TYPE_GROUND,
    Z_TILE_TYPE_PEBBLES,
    Z_TILE_TYPE_NUM
} ZTileType;

extern void z_tiles_load(void);

extern ASprite* z_tiles_sprite(size_t Index);
