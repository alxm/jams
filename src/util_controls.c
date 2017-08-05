/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_controls.h"

ZUtilControls z_util_controls;

void z_util_controls_load(void)
{
    z_util_controls.up = a_button_new("key.up gamepad.b.up");
    z_util_controls.down = a_button_new("key.down gamepad.b.down");
    z_util_controls.left = a_button_new("key.left gamepad.b.left");
    z_util_controls.right = a_button_new("key.right gamepad.b.right");
}
