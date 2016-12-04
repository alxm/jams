#include <a2x.h>

#include "controls.h"
#include "graphics.h"
#include "tiles.h"

A_STATE(load)
{
    A_STATE_INIT
    {
        z_controls_load();
        z_graphics_load();
        z_tiles_load();

        a_state_push("world");
    }

    A_STATE_FREE
    {
        z_graphics_free();
    }
}
