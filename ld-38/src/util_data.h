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
    Z_DATA_YEAR,
    Z_DATA_POPULATION,
    Z_DATA_DEFENSE,
    Z_DATA_HEALTH,
    Z_DATA_INDUSTRY,
    Z_DATA_SCIENCE,
    Z_DATA_NUM
} ZDataType;

typedef struct ZData ZData;

extern void z_data_load(void);

extern ZData* z_data_new(void);
extern void z_data_free(ZData* Data);

extern int z_data_getReal(const ZData* Data, ZDataType Type);
extern int z_data_getShow(const ZData* Data, ZDataType Type);
extern void z_data_set(ZData* Data, ZDataType Type, int Value);
extern void z_data_inc(ZData* Data, ZDataType Type, int Inc);

extern bool z_data_isMin(ZData* Data, ZDataType Type);
extern bool z_data_isMax(ZData* Data, ZDataType Type);
extern const char* z_data_getName(ZDataType Type);

extern bool z_data_tick(ZData* Data, bool Smooth);
