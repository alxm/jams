#include <a2x.h>

#include "controls.h"

ZControls z_controls;

void z_controls_load(void)
{
    z_controls.up = a_input_new("generic.up");
    z_controls.down = a_input_new("generic.down");
    z_controls.left = a_input_new("generic.left");
    z_controls.right = a_input_new("generic.right");
}
