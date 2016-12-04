#include <a2x.h>

#include "component_mapevent.h"

struct ZCompMapEvent {
    int q;
};

size_t z_comp_mapevent_size(void)
{
    return sizeof(ZCompMapEvent);
}

void z_comp_mapevent_init(ZCompMapEvent* Event)
{
    Event->q = 0;
}
