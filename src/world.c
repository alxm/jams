#include <a2x.h>

struct ZTile {
    bool walkable;
    ASprite* sprite;
};

struct ZMap {
    int w, h;
    int** tiles; // index to tiles
};

struct ZPlayer {
    AFix x, y; // pixel-level coords on world map
    ASpriteFrames* frames;
};

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
