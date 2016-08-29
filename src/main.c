/*
    Copyright 2016 Alex Margarit

    This file is part of Rushed Dinosaur Dare.

    Rushed Dinosaur Dare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Rushed Dinosaur Dare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rushed Dinosaur Dare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "game.h"
#include "load.h"

A_SETUP
{
    a_settings_set("app.title", "Rushed Dinosaur Dare");
    a_settings_set("app.version", "0.1");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "y");
    a_settings_set("app.output.verbose", "y");
    a_settings_set("video.width", "320");
    a_settings_set("video.height", "200");
    a_settings_set("fps.rate", "30");
}

A_MAIN
{
    a_state_new("load", load);
    a_state_new("newgame", newgame);
    a_state_new("rungame", rungame);
    a_state_new("gameover", gameover);

    a_state_push("load");
}
