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

#include "component_health.h"

struct ZCompHealth {
    int points;
    int max;
};

size_t z_comp_health_size(void)
{
    return sizeof(ZCompHealth);
}

void z_comp_health_init(ZCompHealth* Health, int Points)
{
    Health->points = Points;
    Health->max = Points;
}

void z_comp_health_getStats(const ZCompHealth* Health, int* Points, int* Max)
{
    *Points = Health->points;
    *Max = Health->max;
}

int z_comp_health_takeDamage(ZCompHealth* Health, int Damage)
{
    Damage = a_math_max(Damage - 2 + a_random_getInt(5), 0);
    Health->points = a_math_max(Health->points - Damage, 0);

    return Damage;
}

void z_comp_health_addPoints(ZCompHealth* Health, int Points)
{
    Health->points = a_math_min(Health->points + Points, Health->max);
}

bool z_comp_health_isAlive(const ZCompHealth* Health)
{
    return Health->points > 0;
}
