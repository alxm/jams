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

#include "util_data.h"
#include "util_graphics.h"
#include "util_log.h"
#include "util_period.h"

#include "state_disaster.h"
#include "state_game.h"
#include "state_investment.h"
#include "state_load.h"

A_SETUP
{
    a_settings_set("app.title", "38th Century");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("app.output.verbose", "yes");
    a_settings_set("video.borderColor", "0x11 0x11 0x11");
    a_settings_set("video.fps", "30");
    a_settings_set("video.fps.skip", "yes");
}

A_MAIN
{
    a_state_new("load", load);
    a_state_new("title", title);
    a_state_new("play", play);
    a_state_new("over", over);
    a_state_new("hiscore", hiscore);
    a_state_new("evolution", evolution);
    a_state_new("disaster", disaster);
    a_state_new("investment", investment);

    a_state_push("load");
}
