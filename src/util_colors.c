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

#include "util_colors.h"

ZUtilColors z_util_colors;

void z_util_colors_load(void)
{
    z_util_colors.gray1 = a_pixel_hex(0x111111);
    z_util_colors.gray2 = a_pixel_hex(0x222222);
    z_util_colors.gray3 = a_pixel_hex(0xbbbbbb);
    z_util_colors.gray4 = a_pixel_hex(0xdddddd);
    z_util_colors.blue1 = a_pixel_hex(0x009999);
    z_util_colors.blue2 = a_pixel_hex(0x00dddd);
    z_util_colors.red1 = a_pixel_hex(0x990099);
    z_util_colors.red2 = a_pixel_hex(0xdd00dd);
}
