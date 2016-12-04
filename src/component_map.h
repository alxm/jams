typedef struct ZCompMap ZCompMap;

extern size_t z_comp_map_size(void);
extern AComponentFree z_comp_map_free;
extern void z_comp_map_init(ZCompMap* Map);

extern ASprite* z_comp_map_sprite(const ZCompMap* Map, int X, int Y);
extern void z_comp_map_getDim(const ZCompMap* Map, int* W, int* H);
extern bool z_comp_map_canWalk(const ZCompMap* Map, int X, int Y);
