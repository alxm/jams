/*
    Copyright 2016 Alex Margarit

    This file is part of Motel 37.

    Motel 37 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Motel 37 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Motel 37.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <a2x.h>

#include "tiles.h"
#include "world.h"
#include "missions.h"

ZMissions z_missions;

void z_missions_getNewMission(void)
{
    z_missions.north = a_random_int(2);
    z_missions.west = a_random_int(2);
    z_missions.numAttempts = 0;
}

char* z_missions_getMissionClues(void)
{
    static char text[128];

    snprintf(text, 128, "\"The item I want is burried in the %s-%s end",
        z_missions.north ? "NORTH" : "SOUTH",
        z_missions.west ? "WEST" : "EAST");

    return text;
}
