#include <a2x.h>

#include "load.h"
#include "world.h"

A_SETUP
{
    a_settings_set("app.title", "church2");
    a_settings_set("app.version", "1.0");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("video.fps", "30");
}

A_MAIN
{
    a_state_new("load", load);
    a_state_new("world", world);

    a_state_push("load");
}
