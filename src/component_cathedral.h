typedef struct ZCompCathedral ZCompCathedral;

extern size_t z_comp_cathedral_size(void);
extern AComponentFree z_comp_cathedral_free;
extern void z_comp_cathedral_init(ZCompCathedral* Cathedral);

extern ASprite* z_comp_cathedral_getGlass(const ZCompCathedral* Cathedral);
