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

void z_util_bar_draw(APixel Color1, APixel Color2, int Value, int OutOf, int X, int Y, int Width, int Height)
{
    int part1Width = a_math_max(1, a_math_min(Width * Value / OutOf, Width));
    int part2Width = Width - part1Width;

    a_pixel_setPixel(Color1);
    a_draw_rectangle(X, Y, part1Width, Height);

    a_pixel_setPixel(Color2);
    a_draw_rectangle(X + part1Width, Y, part2Width, Height);
}
