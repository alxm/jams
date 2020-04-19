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

#include "o_orb.h"
#include "main.h"

static void h_orb_player(OOrb* Orb)
{
    bool pressed = false;
    unsigned pressMask = 0;

    if(f_button_pressGet(u_input_get(U_BUTTON_UP))) {
        F_FLAGS_SET(pressMask, F_FLAGS_BIT(0));
        pressed = true;
    }

    if(f_button_pressGet(u_input_get(U_BUTTON_DOWN))) {
        F_FLAGS_SET(pressMask, F_FLAGS_BIT(1));
        pressed = true;
    }

    if(f_button_pressGet(u_input_get(U_BUTTON_LEFT))) {
        F_FLAGS_SET(pressMask, F_FLAGS_BIT(2));
        pressed = true;
    }

    if(f_button_pressGet(u_input_get(U_BUTTON_RIGHT))) {
        F_FLAGS_SET(pressMask, F_FLAGS_BIT(3));
        pressed = true;
    }

    static const unsigned angles[F_FLAGS_BIT(4)] = {
        [F_FLAGS_BIT(3)] = 0,
        [F_FLAGS_BIT(0) | F_FLAGS_BIT(3)] = F_DEG_045_INT,
        [F_FLAGS_BIT(0)] = F_DEG_090_INT,
        [F_FLAGS_BIT(0) | F_FLAGS_BIT(2)] = F_DEG_135_INT,
        [F_FLAGS_BIT(2)] = F_DEG_180_INT,
        [F_FLAGS_BIT(1) | F_FLAGS_BIT(2)] = F_DEG_225_INT,
        [F_FLAGS_BIT(1)] = F_DEG_270_INT,
        [F_FLAGS_BIT(1) | F_FLAGS_BIT(3)] = F_DEG_315_INT,
    };

    if(pressed) {
        Orb->physics.angle = angles[pressMask];
    }
}

static void h_orb_npc(OOrb* Orb)
{
    if(f_timer_expiredGet(Orb->physics.timer)) {
        if(f_random_chance(1, 2)) {
            Orb->physics.turnL = true;
            Orb->physics.turnR = false;
        } else {
            Orb->physics.turnL = false;
            Orb->physics.turnR = true;
        }

        Orb->physics.turnInc = f_random_rangeu(
                                F_DEG_001_INT, F_DEG_001_INT * 3);
    }
}

static const OOrbType g_types[O_ORB_TYPE_NUM] = {
    [O_ORB_TYPE_PLAYER] = {
        .radius = F_FIX_ONE * 32 / 256,
        .color = {0xff, 0, 0},
        .speedMax = F_FIX_ONE * 2 / 128,
        .tick = h_orb_player
    },

    [O_ORB_TYPE_NPC1] = {
        .radius = F_FIX_ONE * 16 / 256,
        .color = {0, 0xff, 0},
        .speedMax = F_FIX_ONE * 1 / 128,
        .tick = h_orb_npc
    },

    [O_ORB_TYPE_NPC2] = {
        .radius = F_FIX_ONE * 48 / 256,
        .color = {0, 0, 0xff},
        .speedMax = F_FIX_ONE * 1 / 128,
        .tick = h_orb_npc
    },
};

OOrb* o_orb_new(OOrbTypeId Type, FFix X, FFix Y, unsigned Angle)
{
    OOrb* o = f_mem_mallocz(sizeof(OOrb));

    o->type = &g_types[Type];
    o->coords.x = X;
    o->coords.y = Y;
    o->offset = f_random_intu(F_FIX_ANGLES_NUM);
    o->physics.timer = f_timer_new(F_TIMER_MS, 500, true);
    o->physics.angle = Angle;

    f_timer_runStart(o->physics.timer);

    return o;
}

void o_orb_free(OOrb* Orb)
{
    f_timer_free(Orb->physics.timer);

    f_mem_free(Orb);
}

void o_orb_tick(OOrb* Orb)
{
    Orb->type->tick(Orb);

    Orb->coords.x += Orb->physics.velocity.x;
    Orb->coords.y += Orb->physics.velocity.y;

    Orb->physics.velocity.x += Orb->physics.acceleration.x;
    Orb->physics.velocity.y += Orb->physics.acceleration.y;
    Orb->physics.velocity.x >>= 1;
    Orb->physics.velocity.y >>= 1;

    unsigned angle = Orb->physics.angle;
    FFix magnitude = Orb->type->speedMax;

    Orb->physics.acceleration.x += f_fix_mul(f_fix_cos(angle), magnitude);
    Orb->physics.acceleration.y -= f_fix_mul(f_fix_sin(angle), magnitude);
    Orb->physics.acceleration.x >>= 1;
    Orb->physics.acceleration.y >>= 1;

    if(Orb->physics.turnL) {
        Orb->physics.angle += Orb->physics.turnInc;
    } else if(Orb->physics.turnR) {
        Orb->physics.angle -= Orb->physics.turnInc;
    }
}

void o_orb_draw(OOrb* Orb)
{
    FVecInt coords = n_cam_coordsToScreen(Orb->coords);
    int zoom = n_cam_zoomGet();

    f_color_colorSetRgb(
        Orb->type->color.r, Orb->type->color.g, Orb->type->color.b);

    f_color_fillDrawSet(false);
    f_color_blendSet(F_COLOR_BLEND_ALPHA_50);

    FFix radius = Orb->type->radius * zoom;

    f_draw_circle(
        coords.x,
        coords.y,
        f_fix_toInt(radius + f_fix_mul(radius >> 2,
                                       f_fps_ticksSin(
                                        1, 3, Orb->offset + F_DEG_022_INT))));

    f_color_blendSet(F_COLOR_BLEND_ALPHA_25);

    f_draw_circle(
        coords.x,
        coords.y,
        f_fix_toInt(radius + f_fix_mul(radius >> 1,
                                       f_fps_ticksSin(
                                        1, 2, Orb->offset + F_DEG_045_INT))));

    f_color_fillDrawSet(true);
    f_color_blendSet(F_COLOR_BLEND_ALPHA);
    f_color_alphaSet(
        F_COLOR_ALPHA_MAX / 4
            + f_fix_toInt(
                f_fps_ticksSin(1, 2, Orb->offset) * F_COLOR_ALPHA_MAX / 8));

    f_draw_circle(
        coords.x,
        coords.y,
        f_fix_toInt(radius + f_fix_mul(radius >> 2,
                                       f_fps_ticksSin(1, 4, Orb->offset))));
}
