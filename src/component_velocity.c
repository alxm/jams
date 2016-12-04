#include <a2x.h>

#include "component_velocity.h"

struct ZCompVelocity {
    int speed;
};

size_t z_comp_velocity_size(void)
{
    return sizeof(ZCompVelocity);
}

void z_comp_velocity_init(ZCompVelocity* Velocity)
{
    Velocity->speed = 1;
}

int z_comp_velocity_getSpeed(const ZCompVelocity* Velocity)
{
    return Velocity->speed;
}
