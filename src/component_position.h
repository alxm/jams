typedef struct ZCompPosition ZCompPosition;

extern size_t z_comp_position_size(void);
extern void z_comp_position_init(ZCompPosition* Position, int X, int Y);
extern void z_comp_position_initPointer(ZCompPosition* Position, ZCompPosition* Anchor);

extern void z_comp_position_setCoords(ZCompPosition* Position, int X, int Y);
extern void z_comp_position_getCoords(const ZCompPosition* Position, int* X, int* Y);
extern void z_comp_position_setDir(ZCompPosition* Position, int DirX, int DirY);
extern void z_comp_position_retrieveDir(ZCompPosition* Position, int* DirX, int* DirY);
