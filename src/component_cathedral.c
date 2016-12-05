#include <a2x.h>

#include "component_cathedral.h"
#include "graphics.h"

struct ZCompCathedral {
    bool colorsOn;
    uint8_t angle;
};

size_t z_comp_cathedral_size(void)
{
    return sizeof(ZCompCathedral);
}

void z_comp_cathedral_init(ZCompCathedral* Cathedral)
{
    Cathedral->colorsOn = false;
    Cathedral->angle = 0;
}

ASprite* z_comp_cathedral_getColors(const ZCompCathedral* Cathedral)
{
    if(Cathedral->colorsOn) {
        return z_graphics_getStill("circleColors");
    } else {
        return z_graphics_getStill("circleB&W");
    }
}

void z_comp_cathedral_setColors(ZCompCathedral* Cathedral, bool ColorsOn)
{
    Cathedral->colorsOn = ColorsOn;
}

void z_comp_cathedral_incAngle(ZCompCathedral* Cathedral)
{
    Cathedral->angle++;
}

uint8_t z_comp_cathedral_getAngle(const ZCompCathedral* Cathedral)
{
    return Cathedral->angle;
}
