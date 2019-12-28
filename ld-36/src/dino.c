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

#include "dino.h"

#define DINO_MOVE_RANGE 16
#define DINO_ATTACK_RANGE 48

struct DinoInstance {
    LandInstance* land;
    MovingObject* mover;
    ASprite* body;
    bool moving;
    int xTarget;
};

static void stegoSetLegOffsets(int NumLegs, int* LegX, int* LegY)
{
    for(int i = NumLegs; i--; ) {
        LegY[i] = 18;
    }

    LegX[0] = 9;
    LegX[1] = 15;
    LegX[2] = 29;
    LegX[3] = 36;
}

DinoInstance* dino_new(LandInstance* Land, int NumLegs)
{
    DinoInstance* d = a_mem_malloc(sizeof(DinoInstance));

    int startX = land_getBufferOffset(Land) + a_screen_width();
    int startY = a_screen_height() / 2;

    d->land = Land;
    d->mover = mover_new(Land, startX, startY, NumLegs, stegoSetLegOffsets);
    d->body = g_data.stegoBody;
    d->moving = false;
    d->xTarget = 0;

    for(int i = NumLegs; i--; ) {
        mover_addLegFrames(d->mover, g_data.stegoLegs);
    }

    return d;
}

void dino_free(DinoInstance* Dino)
{
    mover_free(Dino->mover);
    free(Dino);
}

void dinos_tick(AList* Dinos)
{
    if(a_list_empty(Dinos)) {
        DinoInstance* d = dino_new(g_game.ground, 4);
        a_list_addLast(Dinos, d);
    }

    A_LIST_ITERATE(Dinos, DinoInstance*, d) {
        int x, y;
        mover_tick(d->mover);
        mover_getCoords(d->mover, &x, &y);

        if(d->moving) {
            mover_moveX(d->mover, (x < d->xTarget ? 1 : -2) * land_getSpeed(d->land));
            mover_getCoords(d->mover, &x, &y);

            if(x == d->xTarget) {
                d->moving = false;
            }
        } else if(x - buggy_getX(g_game.buggy) <= DINO_ATTACK_RANGE || x > land_getBufferOffset(d->land) + a_screen_width() - DINO_ATTACK_RANGE) {
            d->xTarget = x - DINO_MOVE_RANGE;
            d->moving = true;
        } else if(a_random_int(20) == 0) {
            d->xTarget = x - DINO_MOVE_RANGE + a_random_int(2 * DINO_MOVE_RANGE);
            d->moving = true;
        }
    }
}

void dinos_draw(AList* Dinos)
{
    A_LIST_ITERATE(Dinos, DinoInstance*, d) {
        int x, y;
        mover_getCoords(d->mover, &x, &y);
        x -= land_getBufferOffset(d->land);

        a_blit(d->body, x, y);
        mover_drawLegs(d->mover);
    }
}

bool dino_collidesWith(DinoInstance* Dino, int X, int Y, int W, int H)
{
    int x, y;
    mover_getCoords(Dino->mover, &x, &y);

    return a_collide_boxes(x, y, a_sprite_w(Dino->body), a_sprite_h(Dino->body),
                           X, Y, W, H);
}
