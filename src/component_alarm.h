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

typedef struct ZCompAlarm ZCompAlarm;

extern size_t z_comp_alarm_size(void);
extern void z_comp_alarm_init(ZCompAlarm* Alarm);
extern AComponentFree z_comp_alarm_free;

extern AFix z_comp_alarm_getValue(const ZCompAlarm* Alarm);
extern void z_comp_alarm_setValue(ZCompAlarm* Alarm, AFix Value);

extern bool z_comp_alarm_isOn(const ZCompAlarm* Alarm);
extern void z_comp_alarm_toggle(ZCompAlarm* Alarm, bool On);
