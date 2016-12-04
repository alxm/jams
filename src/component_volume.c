#include <a2x.h>

#include "component_volume.h"

struct ZCompVolume {
    int dim;
    AColObject* colobject;
};

size_t z_comp_volume_size(void)
{
    return sizeof(ZCompVolume);
}

void z_comp_volume_free(void* Self)
{
    ZCompVolume* volume = Self;
    a_colobject_free(volume->colobject);
}

void z_comp_volume_init(ZCompVolume* Volume, int Dim, AColMap* Colmap, int X, int Y)
{
    Volume->dim = Dim;
    Volume->colobject = a_colobject_new(Colmap, Volume);
    a_colobject_setCoords(Volume->colobject, X, Y);
}

int z_comp_volume_getDim(const ZCompVolume* Volume)
{
    return Volume->dim;
}

void z_comp_volume_setCoords(const ZCompVolume* Volume, int X, int Y)
{
    a_colobject_setCoords(Volume->colobject, X, Y);
}
