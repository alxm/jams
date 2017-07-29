/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Despot 3900 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Despot 3900 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Despot 3900.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_controls.h"

ZControls z_controls;

void z_controls_load(void)
{
    z_controls.up = a_button_new("key.up gamepad.b.up");
    z_controls.down = a_button_new("key.down gamepad.b.down");
    z_controls.left = a_button_new("key.left gamepad.b.left");
    z_controls.right = a_button_new("key.right gamepad.b.right");
    z_controls.action = a_button_new("key.space gamepad.b.a");
}

void z_controls_release(void)
{
    a_button_release(z_controls.up);
    a_button_release(z_controls.down);
    a_button_release(z_controls.left);
    a_button_release(z_controls.right);
    a_button_release(z_controls.action);
}
