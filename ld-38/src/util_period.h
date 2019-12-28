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

typedef enum {
    Z_DISASTER_DISEASE,
    Z_DISASTER_CALAMITY,
    Z_DISASTER_ATTACK,
    Z_DISASTER_NUM
} ZDisasterType;

typedef struct ZDisaster {
    ZDisasterType type;
    int damage;
} ZDisaster;

typedef enum {
    Z_PERIOD_CAVES,
    Z_PERIOD_VILLAGE,
    Z_PERIOD_CITY,
    Z_PERIOD_FUTURE,
    Z_PERIOD_NUM
} ZPeriodType;

extern void z_period_load(void);

extern const char* z_period_getId(ZPeriodType Period);
extern int z_period_getDamage(ZPeriodType Period);
