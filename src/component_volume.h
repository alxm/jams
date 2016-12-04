typedef struct ZCompVolume ZCompVolume;

extern size_t z_comp_volume_size(void);
extern AComponentFree z_comp_volume_free;
extern void z_comp_volume_init(ZCompVolume* Volume, int Dim, AColMap* Colmap, int X, int Y);

extern int z_comp_volume_getDim(const ZCompVolume* Volume);
extern AColObject* z_comp_volume_getColobj(const ZCompVolume* Volume);
extern void z_comp_volume_setCoords(const ZCompVolume* Volume, int X, int Y);
