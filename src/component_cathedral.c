#include <a2x.h>

#include "component_cathedral.h"
#include "graphics.h"

struct ZCompCathedral {
    uint8_t angle;
};

size_t z_comp_cathedral_size(void)
{
    return sizeof(ZCompCathedral);
}

void z_comp_cathedral_init(ZCompCathedral* Cathedral)
{
    Cathedral->angle = 0;
}

void z_comp_cathedral_incAngle(ZCompCathedral* Cathedral)
{
    Cathedral->angle++;
}

uint8_t z_comp_cathedral_getAngle(const ZCompCathedral* Cathedral)
{
    return Cathedral->angle;
}
