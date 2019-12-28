/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

#define Z_UTIL_COLOR_LEVELS 3

typedef enum {
    Z_UTIL_COLOR_INVALID = -1,
    Z_UTIL_COLOR_GRAY_LAND,
    Z_UTIL_COLOR_GRAY_UNITS,
    Z_UTIL_COLOR_PURPLE,
    Z_UTIL_COLOR_PINK,
    Z_UTIL_COLOR_GREEN,
    Z_UTIL_COLOR_NUM
} ZUtilColor;

extern void z_util_colors_load(void);

extern APixel z_util_colors_get(ZUtilColor Color, int Level);
