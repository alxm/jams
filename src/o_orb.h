/*
    Copyright 2020 Alex Margarit <alex@alxm.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <faur.h>

typedef struct OOrb OOrb;
typedef struct OOrbType OOrbType;

typedef enum {
    O_ORB_TYPE_INVALID = -1,
    O_ORB_TYPE_PLAYER,
    O_ORB_TYPE_NPC_GOOD,
    O_ORB_TYPE_NPC_POISON,
    O_ORB_TYPE_NUM
} OOrbTypeId;

typedef enum {
    O_ORB_STATE_INVALID = -1,
    O_ORB_STATE_DRIFT,
    O_ORB_STATE_TURN_LEFT,
    O_ORB_STATE_TURN_RIGHT,
    O_ORB_STATE_FOLLOW,
    O_ORB_STATE_CAPTURED,
    O_ORB_STATE_DEAD,
    O_ORB_STATE_NUM
} OOrbStateId;

typedef void OOrbTickHandler(OOrb* Orb);

struct OOrbType {
    FFix radius;
    uint32_t color1, color2;
    FFix speedMax;
    int speedFollowMult;
    FFix followThreshold;
    int lifeFull;
    OOrbTickHandler* tick;
};

struct OOrb {
    const OOrbType* type;
    FVecFix coords, origin;
    unsigned offset;
    int life;
    struct {
        OOrbStateId id;
        FTimer* timer;
        unsigned angle;
    } state;
    struct {
        FVecFix acceleration;
        FVecFix velocity;
        unsigned angle;
        unsigned turnInc;
    } physics;
};

extern OOrb* o_orb_new(OOrbTypeId Type, FFix X, FFix Y, unsigned Angle);
extern void o_orb_free(OOrb* Orb);

extern void o_orb_tick(OOrb* Orb);
extern void o_orb_draw0(OOrb* Orb);
extern void o_orb_draw(OOrb* Orb);
