/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    38th Century - A simple world simulation game made for Ludum Dare 38 72h

    38th Century is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    38th Century is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 38th Century.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "util_period.h"

typedef struct ZPeriod {
    const char* id;
    int damage;
} ZPeriod;

static ZPeriod g_periods[Z_PERIOD_NUM];

void z_period_load(void)
{
    g_periods[Z_PERIOD_CAVES].id = "cave";
    g_periods[Z_PERIOD_CAVES].damage = 64;
}

const char* z_period_getId(ZPeriodType Period)
{
    return g_periods[Period].id;
}

int z_period_getDamage(ZPeriodType Period)
{
    return g_periods[Period].damage;
}
