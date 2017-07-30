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

#include "util_despot.h"

struct ZDespot {
    int dobInMonths;
    int health;
    int wealth;
    int popularity;
    int loyalty;
};

ZDespot* z_despot_new(int DobInMonths, int Health, int Wealth, int Popularity, int Loyalty)
{
    ZDespot* d = a_mem_malloc(sizeof(ZDespot));

    d->dobInMonths = DobInMonths;
    d->health = Health;
    d->wealth = Wealth;
    d->popularity = Popularity;
    d->loyalty = Loyalty;

    return d;
}

void z_despot_free(ZDespot* Despot)
{
    free(Despot);
}


int z_despot_getDobInMonths(ZDespot* Despot)
{
    return Despot->dobInMonths;
}

int z_despot_getHealth(ZDespot* Despot)
{
    return Despot->health;
}

void z_despot_setHealth(ZDespot* Despot, int Health)
{
    Despot->health = Health;
}

int z_despot_getWealth(ZDespot* Despot)
{
    return Despot->wealth;
}

int z_despot_getPopularity(ZDespot* Despot)
{
    return Despot->popularity;
}

int z_despot_getLoyalty(ZDespot* Despot)
{
    return Despot->loyalty;
}
