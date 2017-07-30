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

#include "state_game.h"

#include "util_despot.h"

struct ZDespot {
    ZGame* game;
    int dobInMonths;
    int health;
    int wealth;
    int popularity;
    int loyalty;
};

ZDespot* z_despot_new(ZGame* Game, int DobInMonths, int Health, int Wealth, int Popularity, int Loyalty)
{
    ZDespot* d = a_mem_malloc(sizeof(ZDespot));

    d->game = Game;
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

int z_despot_getDobInMonths(const ZDespot* Despot)
{
    return Despot->dobInMonths;
}

int z_despot_getHealth(const ZDespot* Despot)
{
    return Despot->health;
}

void z_despot_setHealth(ZDespot* Despot, int Health)
{
    Despot->health = Health;
}

int z_despot_getWealth(const ZDespot* Despot)
{
    return Despot->wealth;
}

void z_despot_setWealth(ZDespot* Despot, int Wealth)
{
    Despot->wealth = Wealth;
}

int z_despot_getPopularity(const ZDespot* Despot)
{
    return Despot->popularity;
}

void z_despot_setPopularity(ZDespot* Despot, int Popularity)
{
    Despot->popularity = Popularity;
}

int z_despot_getLoyalty(const ZDespot* Despot)
{
    return Despot->loyalty;
}

void z_despot_setLoyalty(ZDespot* Despot, int Loyalty)
{
    Despot->loyalty = Loyalty;
}
