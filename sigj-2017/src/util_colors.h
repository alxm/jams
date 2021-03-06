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

typedef struct ZUtilColors ZUtilColors;

struct ZUtilColors {
    APixel gray1;
    APixel gray2;
    APixel gray3;
    APixel gray4;
    APixel blue1;
    APixel blue2;
    APixel red1;
    APixel red2;
};

extern ZUtilColors z_util_colors;

extern void z_util_colors_load(void);
