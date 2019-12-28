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

#include "buggy.h"

#define BUGGY_WIDTH 27 // TODO use sprite values
#define BUGGY_HEIGHT 25
#define JUMPING_THRESHOLD 4

struct BuggyInstance {
    LandInstance* land;
    MovingObject* mover;
    int cabinXOffset;
    bool moveLeft;
    bool moveRight;
    bool jump;
    bool jumping;
    int jumpingHeight;
    bool kickBack;
    AFix kickBackAccel;
};

static void buggySetTrackOffsets(int NumLegs, int* LegX, int* LegY)
{
    for(int i = NumLegs; i--; ) {
        LegY[i] = a_sprite_h(g_data.buggyCabin) + 3;
    }

    LegX[0] = 0;
    LegX[1] = 21;
}

BuggyInstance* buggy_new(LandInstance* Land)
{
    BuggyInstance* b = a_mem_malloc(sizeof(BuggyInstance));

    b->land = Land;
    b->mover = mover_new(Land, land_getBufferOffset(Land) + 8, a_screen_height() / 2, 2, buggySetTrackOffsets);
    b->cabinXOffset = 0;
    b->moveLeft = false;
    b->moveRight = false;
    b->jump = false;
    b->jumping = false;
    b->jumpingHeight = 0;
    b->kickBack = false;
    b->kickBackAccel = 0;

    mover_addLegFrames(b->mover, g_data.buggyBackTracks);
    mover_addLegFrames(b->mover, g_data.buggyFrontTracks);

    return b;
}

void buggy_free(BuggyInstance* Buggy)
{
    mover_free(Buggy->mover);
    free(Buggy);
}

void buggy_tick(BuggyInstance* Buggy)
{
    mover_tick(Buggy->mover);

    int x, y;
    mover_getCoords(Buggy->mover, &x, &y);

    A_LIST_ITERATE(g_game.dinos, DinoInstance*, d) {
        if(dino_collidesWith(d, x, y, BUGGY_WIDTH, BUGGY_HEIGHT)) {
            Buggy->kickBack = true;
            Buggy->kickBackAccel = 6 * A_FIX_ONE;
            break;
        }
    }

    if(Buggy->kickBack) {
        mover_moveX(Buggy->mover, a_fix_mul(-Buggy->kickBackAccel, land_getSpeed(Buggy->land)));
        Buggy->kickBackAccel = a_fix_mul(Buggy->kickBackAccel, A_FIX_ONE - A_FIX_ONE / 8);

        if(Buggy->kickBackAccel < A_FIX_ONE / 2) {
            Buggy->kickBack = false;
        } else {
            return;
        }
    }

    Buggy->cabinXOffset = 0;

    if(Buggy->moveLeft && x - land_getBufferOffset(Buggy->land) > 0 - a_sprite_w(g_data.buggyCabin) / 4) {
        mover_moveX(Buggy->mover, -land_getSpeed(Buggy->land));
        Buggy->cabinXOffset = -1;
        mover_pauseFrames(Buggy->mover);
    } else {
        mover_resumeFrames(Buggy->mover);
    }

    if(Buggy->moveRight && x - land_getBufferOffset(Buggy->land) < a_screen_width() - (a_sprite_w(g_data.buggyCabin) - a_sprite_w(g_data.buggyCabin) / 4)) {
        mover_moveX(Buggy->mover, land_getSpeed(Buggy->land));
        Buggy->cabinXOffset = 2;
        mover_setFramesSpeed(Buggy->mover, 2);
    } else {
        mover_setFramesSpeed(Buggy->mover, 6);
    }

    if(Buggy->jump && !Buggy->jumping) {
        if(land_getTopY(Buggy->land, x) - (y + BUGGY_HEIGHT) < 2) {
            Buggy->jumping = true;
        }
    }

    if(Buggy->jumping) {
        if(Buggy->jumpingHeight < JUMPING_THRESHOLD) {
            Buggy->jumpingHeight++;
            mover_jump(Buggy->mover);
        } else {
            Buggy->jumpingHeight = 0;
            Buggy->jumping = false;
        }
    }

    Buggy->moveLeft = false;
    Buggy->moveRight = false;
    Buggy->jump = false;
}

void buggy_draw(BuggyInstance* Buggy)
{
    int x, y;
    mover_getCoords(Buggy->mover, &x, &y);
    x -= land_getBufferOffset(Buggy->land);

    a_blit(g_data.buggyCabin, x + Buggy->cabinXOffset, y);
    a_blit(g_data.buggyChassis, x + 2, y + a_sprite_h(g_data.buggyCabin) + 1);
    mover_drawLegs(Buggy->mover);
}

int buggy_getX(BuggyInstance* Buggy)
{
    int x, y;
    mover_getCoords(Buggy->mover, &x, &y);

    return x + BUGGY_WIDTH;
}

void buggy_moveLeft(BuggyInstance* Buggy)
{
    Buggy->moveLeft = true;
}

void buggy_moveRight(BuggyInstance* Buggy)
{
    Buggy->moveRight = true;
}

void buggy_jump(BuggyInstance* Buggy)
{
    Buggy->jump = true;
}

void buggy_shoot(BuggyInstance* Buggy)
{
    //
}
