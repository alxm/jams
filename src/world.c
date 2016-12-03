#include <a2x.h>

#include "map.h"

typedef struct ZPlayer {
    AFix x, y; // pixel-level coords on world map
    ASpriteFrames* frames;
} ZPlayer;

typedef struct ZGame {
    ZMap* map;
    ZPlayer* player;
} ZGame;

A_STATE(world)
{
    A_STATE_INIT
    {
        //
    }

    A_STATE_BODY
    {
        printf("whahaa\n");

        A_STATE_LOOP
        {
            //
        }
    }

    A_STATE_FREE
    {
        //
    }
}
