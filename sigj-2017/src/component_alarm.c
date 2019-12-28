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

#include "component_alarm.h"

struct ZCompAlarm {
    AFix value;
    bool on;
};

size_t z_comp_alarm_size(void)
{
    return sizeof(ZCompAlarm);
}

AFix z_comp_alarm_getValue(const ZCompAlarm* Alarm)
{
    return Alarm->value;
}

void z_comp_alarm_setValue(ZCompAlarm* Alarm, AFix Value)
{
    Alarm->value = Value;
}

bool z_comp_alarm_isOn(const ZCompAlarm* Alarm)
{
    return Alarm->on;
}

void z_comp_alarm_toggle(ZCompAlarm* Alarm, bool On)
{
    Alarm->on = On;
}
