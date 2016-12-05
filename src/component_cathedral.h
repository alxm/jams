typedef struct ZCompCathedral ZCompCathedral;

extern size_t z_comp_cathedral_size(void);
extern void z_comp_cathedral_init(ZCompCathedral* Cathedral);

extern ASprite* z_comp_cathedral_getColors(const ZCompCathedral* Cathedral);
extern void z_comp_cathedral_setColors(ZCompCathedral* Cathedral, bool ColorsOn);
extern void z_comp_cathedral_incAngle(ZCompCathedral* Cathedral);
extern uint8_t z_comp_cathedral_getAngle(const ZCompCathedral* Cathedral);
