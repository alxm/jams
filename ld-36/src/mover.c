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

#include "mover.h"

struct MovingObject {
    LandInstance* land;
    AFix x;
    int y;
    int numLegs;
    int* legX;
    int* legY;
    int* legYOffset;
    ASpriteFrames** legFrames;
    int numSetLegFrames;
};

MovingObject* mover_new(LandInstance* Land, int StartX, int StartY, int NumLegs, MoverSetLegOffsets LegOffsetsCallback)
{
    MovingObject* m = a_mem_malloc(sizeof(MovingObject));

    m->land = Land;
    m->x = a_fix_itofix(StartX);
    m->y = StartY;
    m->numLegs = NumLegs;
    m->legX = a_mem_malloc(NumLegs * sizeof(int));
    m->legY = a_mem_malloc(NumLegs * sizeof(int));
    m->legYOffset = a_mem_malloc(NumLegs * sizeof(int));
    m->legFrames = a_mem_malloc(NumLegs * sizeof(ASpriteFrames*));
    m->numSetLegFrames = 0;

    for(int i = 0; i < NumLegs; i++) {
        m->legYOffset[i] = 0;
    }

    LegOffsetsCallback(NumLegs, m->legX, m->legY);

    return m;
}

void mover_free(MovingObject* Mover)
{
    free(Mover->legX);
    free(Mover->legY);
    free(Mover->legYOffset);

    for(int i = 0; i < Mover->numLegs; i++) {
        a_spriteframes_free(Mover->legFrames[i], false);
    }

    free(Mover->legFrames);
    free(Mover);
}

void mover_addLegFrames(MovingObject* Mover, ASpriteFrames* LegFrames)
{
    ASpriteFrames* frames = a_spriteframes_clone(LegFrames);

    if(Mover->numSetLegFrames == 0 || Mover->numSetLegFrames == Mover->numLegs - 1) {
        for(int j = a_spriteframes_getSpeed(frames); j--; ) {
            a_spriteframes_next(frames);
        }
    }

    Mover->legFrames[Mover->numSetLegFrames++] = frames;
}

void mover_getCoords(MovingObject* Mover, int* X, int* Y)
{
    *X = a_fix_fixtoi(Mover->x);
    *Y = Mover->y;
}

void mover_moveX(MovingObject* Mover, AFix Dx)
{
    Mover->x += Dx;
}

void mover_jump(MovingObject* Mover)
{
    Mover->y -= 2;
}

void mover_tick(MovingObject* Mover)
{
    int numLegsGoingUp = 0;
    int numLegsGoingDown = 0;
    bool goingUp[Mover->numLegs];
    bool goingDown[Mover->numLegs];

    memset(goingUp, 0, Mover->numLegs * sizeof(bool));
    memset(goingDown, 0, Mover->numLegs * sizeof(bool));

    for(int i = Mover->numLegs; i--; ) {
        ASprite* legSprite = a_spriteframes_next(Mover->legFrames[i]);
        int x = a_fix_fixtoi(Mover->x) + Mover->legX[i] + a_sprite_w(legSprite) / 2;
        int y = Mover->y + Mover->legY[i] + a_sprite_h(legSprite);
        int landY = land_getTopY(Mover->land, x);

        if(y < landY) {
            numLegsGoingDown++;
            goingDown[i] = true;
            Mover->legYOffset[i] = 1;
        } else if(y > landY) {
            numLegsGoingUp++;
            goingUp[i] = true;
            Mover->legYOffset[i] = -1;
        }
    }

    if(numLegsGoingUp == Mover->numLegs) {
        Mover->y--;
        memset(Mover->legYOffset, 0, Mover->numLegs * sizeof(int));
    } else if(numLegsGoingDown == Mover->numLegs) {
        Mover->y++;
        memset(Mover->legYOffset, 0, Mover->numLegs * sizeof(int));
    }
}

void mover_drawLegs(MovingObject* Mover)
{
    for(int i = Mover->numLegs; i--; ) {
        a_blit(a_spriteframes_get(Mover->legFrames[i]),
               a_fix_fixtoi(Mover->x) + Mover->legX[i] - land_getBufferOffset(Mover->land),
               Mover->y + Mover->legY[i] + Mover->legYOffset[i]);
    }
}

void mover_pauseFrames(MovingObject* Mover)
{
    for(int i = Mover->numLegs; i--; ) {
        a_spriteframes_pause(Mover->legFrames[i]);
    }
}

void mover_resumeFrames(MovingObject* Mover)
{
    for(int i = Mover->numLegs; i--; ) {
        a_spriteframes_resume(Mover->legFrames[i]);
    }
}

void mover_setFramesSpeed(MovingObject* Mover, unsigned int CallsToNextFrame)
{
    for(int i = Mover->numLegs; i--; ) {
        a_spriteframes_setSpeed(Mover->legFrames[i], CallsToNextFrame);
    }
}
