/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_controls.h"

ZControls z_controls;

void z_controls_load(void)
{
    z_controls.up = a_button_new("generic.up");
    z_controls.down = a_button_new("generic.down");
    z_controls.left = a_button_new("generic.left");
    z_controls.right = a_button_new("generic.right");
    z_controls.main = a_button_new("generic.b0");
    z_controls.secondary = a_button_new("generic.b1");

    unsigned repeatInterval = a_settings_getUnsigned("video.fps") / 5;

    a_button_setRepeat(z_controls.up, repeatInterval);
    a_button_setRepeat(z_controls.down, repeatInterval);
    a_button_setRepeat(z_controls.left, repeatInterval);
    a_button_setRepeat(z_controls.right, repeatInterval);
}
