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

#include "load.h"
#include "state_graveyard.h"
#include "state_motelroom1.h"
#include "state_parkinglot.h"

A_SETUP
{
    a_settings_set("app.title", "Motel 37");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("app.output.verbose", "yes");
    a_settings_set("video.fps", "30");
}

A_MAIN
{
    a_state_new("load", load);
    a_state_new("parkingLot", parkingLot);
    a_state_new("graveyard", graveyard);
    a_state_new("motelRoom1", motelRoom1);

    a_state_push("load");
}
