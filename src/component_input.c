#include <a2x.h>

#include "component_input.h"

struct ZCompInput {
    ZCompInputHandler* handler;
};

size_t z_comp_input_size(void)
{
    return sizeof(ZCompInput);
}

void z_comp_input_init(ZCompInput* Input, ZCompInputHandler* Handler)
{
    Input->handler = Handler;
}

void z_comp_input_callHandler(ZCompInput* Input)
{
    Input->handler(a_component_getEntity(Input));
}
