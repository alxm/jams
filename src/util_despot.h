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

extern ZDespot* z_despot_new(ZGame* Game, int DobInMonths, int Health, int Wealth, int Popularity, int Loyalty);
extern void z_despot_free(ZDespot* Despot);

extern int z_despot_getAgeInYears(const ZDespot* Despot);

extern int z_despot_getHealth(const ZDespot* Despot);
extern void z_despot_setHealth(ZDespot* Despot, int Health);

extern int z_despot_getWealth(const ZDespot* Despot);
extern void z_despot_setWealth(ZDespot* Despot, int Wealth);

extern int z_despot_getPopularity(const ZDespot* Despot);
extern void z_despot_setPopularity(ZDespot* Despot, int Popularity);

extern int z_despot_getLoyalty(const ZDespot* Despot);
extern void z_despot_setLoyalty(ZDespot* Despot, int Loyalty);
