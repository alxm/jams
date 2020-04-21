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
    F_UNUSED(Orb);
}

static void h_orb_npc(OOrb* Orb)
{
    F_UNUSED(Orb);
}

static const OOrbType g_types[O_ORB_TYPE_NUM] = {
    [O_ORB_TYPE_PLAYER] = {
        .radius = F_FIX_ONE * 48 / 256,
        .color1 = 0xcdea7d,
        .color2 = 0x63ea82,
        .speedMax = F_FIX_ONE * 2 / 128,
        .speedFollowMult = 1,
        .followThreshold = 0,
        .lifeFull = 1000,
        .ms = 0,
        .tick = h_orb_player
    },

    [O_ORB_TYPE_NPC_GOOD] = {
        .radius = F_FIX_ONE * 32 / 256,
        .color1 = 0x469ed5,
        .color2 = 0x4e3d40,
        .speedMax = F_FIX_ONE * 1 / 128,
        .speedFollowMult = 4,
        .followThreshold = F_FIX_ONE / 3,
        .lifeFull = 50,
        .ms = 800,
        .tick = h_orb_npc
    },

    [O_ORB_TYPE_NPC_POISON] = {
        .radius = F_FIX_ONE * 64 / 256,
        .color1 = 0xed536c,
        .color2 = 0x4e3d40,
        .speedMax = F_FIX_ONE * 1 / 128,
        .speedFollowMult = 8,
        .followThreshold = F_FIX_ONE / 2,
        .lifeFull = 100,
        .ms = 1200,
        .tick = h_orb_npc
    },
};

OOrb* o_orb_new(OOrbTypeId Type, FFix X, FFix Y, unsigned Angle)
{
    OOrb* o = f_mem_mallocz(sizeof(OOrb));

    o->type = &g_types[Type];
    o->coords.x = X;
    o->coords.y = Y;
    o->origin = o->coords;
    o->offset = f_random_intu(F_DEG_360_INT);
    o->life = o->type->lifeFull;

    o->state.id = O_ORB_STATE_DRIFT;
    o->state.timer = f_timer_new(
                        F_TIMER_MS, o->type->ms + f_random_intu(400), true);

    o->physics.angle = Angle;

    f_timer_runStart(o->state.timer);

    return o;
}

void o_orb_free(OOrb* Orb)
{
    f_timer_free(Orb->state.timer);

    f_mem_free(Orb);
}

static inline bool distance_lt(FFix DistanceSquared, FFix Threshold)
{
    return DistanceSquared < f_fix_mul(Threshold, Threshold);
}

static inline bool is_type(const OOrb* Orb, OOrbTypeId Id)
{
    return Orb->type == &g_types[Id];
}

static void logicPlayer(OOrb* Orb)
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

    if(pressed && Orb->physics.angle != angles[pressMask]) {
        Orb->physics.angle = angles[pressMask];

        n_cam_zoomOut();
    }
}

static void logicAi(OOrb* Orb, OOrb* Player)
{
    FVecFix delta = {Orb->coords.x - Player->coords.x,
                     Orb->coords.y - Player->coords.y};
    FFix distance = f_fix_mul(delta.x, delta.x) + f_fix_mul(delta.y, delta.y);

    switch(Orb->state.id) {
        case O_ORB_STATE_DRIFT: {
            if(distance_lt(distance, Orb->type->followThreshold)) {
                Orb->state.id = O_ORB_STATE_FOLLOW;
            } else if(f_timer_expiredGet(Orb->state.timer)) {
                Orb->state.id = f_random_chance(1, 2)
                                    ? O_ORB_STATE_TURN_LEFT
                                    : O_ORB_STATE_TURN_RIGHT;

                Orb->physics.turnInc = f_random_rangeu(
                                        F_DEG_001_INT, F_DEG_001_INT * 3);
            }
        } break;

        case O_ORB_STATE_TURN_LEFT: {
            Orb->physics.angle += Orb->physics.turnInc;

            if(distance_lt(distance, Orb->type->followThreshold)) {
                Orb->state.id = O_ORB_STATE_FOLLOW;
            } else if(f_timer_expiredGet(Orb->state.timer)) {
                Orb->state.id = O_ORB_STATE_DRIFT;
            }
        } break;

        case O_ORB_STATE_TURN_RIGHT: {
            Orb->physics.angle -= Orb->physics.turnInc;

            if(distance_lt(distance, Orb->type->followThreshold)) {
                Orb->state.id = O_ORB_STATE_FOLLOW;
            } else if(f_timer_expiredGet(Orb->state.timer)) {
                Orb->state.id = O_ORB_STATE_DRIFT;
            }
        } break;

        case O_ORB_STATE_FOLLOW: {
            Orb->origin = Player->coords;
            Orb->physics.angle = f_fix_atan(Orb->coords.x,
                                            Orb->coords.y,
                                            Player->coords.x,
                                            Player->coords.y);

            if(distance_lt(distance, Player->type->radius)) {
                if(is_type(Orb, O_ORB_TYPE_NPC_POISON)) {
                    Player->life = f_math_max(Player->life - 100, 0);
                    Orb->life -= 50;

                    if(Orb->life <= 0) {
                        Orb->state.id = O_ORB_STATE_CAPTURED;
                        Orb->state.angle = 0;
                    } else {
                        Orb->state.id = O_ORB_STATE_RETREAT;
                        f_timer_runStart(Orb->state.timer);
                    }

                    u_sound_play(U_SFX_HISS);
                    n_cam_shakeSet(200);
                } else if(is_type(Orb, O_ORB_TYPE_NPC_GOOD)) {
                    Player->life = f_math_min(Player->life + Orb->life,
                                              Player->type->lifeFull);

                    Orb->state.id = O_ORB_STATE_CAPTURED;
                    Orb->state.angle = 0;

                    n_game.orbsGood--;

                    u_sound_play(U_SFX_JINGLE);
                }
            }
        } break;

        case O_ORB_STATE_RETREAT: {
            if(f_timer_expiredGet(Orb->state.timer)) {
                Orb->state.id = O_ORB_STATE_DRIFT;
            } else {
                Orb->physics.angle = f_fix_atan(Orb->coords.x,
                                                Orb->coords.y,
                                                Player->coords.x,
                                                Player->coords.y)
                                        + F_DEG_180_INT;
            }
        } break;

        case O_ORB_STATE_CAPTURED: {
            Orb->origin = Player->coords;
            Orb->state.angle += F_DEG_001_INT * 2;

            if(Orb->state.angle >= F_DEG_090_INT) {
                Orb->state.id = O_ORB_STATE_DEAD;
            }
        } break;

        default: break;
    }
}

static void logic(OOrb* Orb)
{
    if(Orb == n_game.player) {
        logicPlayer(Orb);
    } else {
        logicAi(Orb, n_game.player);
    }
}

static void move(OOrb* Orb)
{
    if(Orb->state.id == O_ORB_STATE_CAPTURED) {
        return;
    }

    FVecFix old = Orb->coords;

    Orb->coords.x += Orb->physics.velocity.x;
    Orb->coords.y += Orb->physics.velocity.y;

    if(Orb->coords.x < 0 || Orb->coords.x >= N_MAP_W * F_FIX_ONE) {
        Orb->coords = old;
        Orb->physics.angle = F_DEG_180_INT - Orb->physics.angle;
        Orb->physics.velocity.x *= -1;
        Orb->physics.acceleration.x *= -1;

        if(Orb == n_game.player) {
            n_cam_zoomOut();
        }
    }

    if(Orb->coords.y < 0 || Orb->coords.y >= N_MAP_H * F_FIX_ONE) {
        Orb->coords = old;
        Orb->physics.angle = -Orb->physics.angle;
        Orb->physics.velocity.y *= -1;
        Orb->physics.acceleration.y *= -1;

        if(Orb == n_game.player) {
            n_cam_zoomOut();
        }
    }

    Orb->physics.velocity.x += Orb->physics.acceleration.x;
    Orb->physics.velocity.y += Orb->physics.acceleration.y;
    Orb->physics.velocity.x >>= 1;
    Orb->physics.velocity.y >>= 1;

    unsigned angle = Orb->physics.angle;
    FFix magnitude = Orb->type->speedMax;

    if(Orb->state.id == O_ORB_STATE_FOLLOW) {
        magnitude *= Orb->type->speedFollowMult;
    }

    Orb->physics.acceleration.x += f_fix_mul(f_fix_cos(angle), magnitude);
    Orb->physics.acceleration.y -= f_fix_mul(f_fix_sin(angle), magnitude);
    Orb->physics.acceleration.x >>= 1;
    Orb->physics.acceleration.y >>= 1;
}

void o_orb_tick(OOrb* Orb)
{
    Orb->type->tick(Orb);
    logic(Orb);
    move(Orb);
}

void o_orb_draw0(OOrb* Orb)
{
    int radius = f_fix_toInt(Orb->type->radius * n_cam_zoomGet()) / 2;
    FVecInt coords = n_cam_coordsToScreen(Orb->coords);
    FVecInt origin = n_cam_coordsToScreen(Orb->origin);

    if(Orb->state.id == O_ORB_STATE_CAPTURED) {
        f_color_colorSetHex(0xcdea7d);
        f_color_blendSet(F_COLOR_BLEND_ALPHA);
        f_color_alphaSet(f_fix_toInt(
                            f_fix_sin(F_DEG_090_INT - Orb->state.angle)
                                * F_COLOR_ALPHA_MAX / 2));
    } else {
        f_color_colorSetHex(0xa6a4ae);
        f_color_blendSet(F_COLOR_BLEND_SOLID);
    }

    f_draw_line(origin.x, origin.y - 2, coords.x, coords.y - radius);
    f_draw_line(origin.x, origin.y + 2, coords.x, coords.y + radius);
    f_draw_line(origin.x - 2, origin.y, coords.x - radius, coords.y);
    f_draw_line(origin.x + 2, origin.y, coords.x + radius, coords.y);
}

void o_orb_draw(OOrb* Orb)
{
    int zoom = n_cam_zoomGet();
    FFix radius = Orb->type->radius * zoom;
    FVecInt coords = n_cam_coordsToScreen(Orb->coords);

    FFix sin12 = f_fps_ticksSin(1, 2, Orb->offset);
    FFix sin13 = f_fps_ticksSin(1, 3, Orb->offset);
    FFix sin14 = f_fps_ticksSin(1, 4, Orb->offset);

    if(Orb->state.id == O_ORB_STATE_CAPTURED) {
        f_color_fillDrawSet(true);
        f_color_blendSet(F_COLOR_BLEND_ALPHA);
        f_color_alphaSet(f_fix_toInt(
                            f_fix_sin(F_DEG_090_INT - Orb->state.angle)
                                * F_COLOR_ALPHA_MAX / 4));

        f_color_colorSetHex(Orb->type->color1);
        f_draw_circle(coords.x,
                      coords.y,
                      f_fix_toInt(radius + f_fix_mul(radius / 2, sin12)));

        f_color_colorSetHex(Orb->type->color2);
        f_draw_circle(coords.x,
                      coords.y,
                      f_fix_toInt(radius / 2 + f_fix_mul(radius / 4, sin14)));
    } else {
        f_color_fillDrawSet(false);

        f_color_colorSetHex(Orb->type->color2);
        f_color_blendSet(F_COLOR_BLEND_ALPHA_50);

        f_draw_circle(coords.x,
                      coords.y,
                      f_fix_toInt(radius + f_fix_mul(radius / 2, sin13)));

        f_color_colorSetHex(Orb->type->color1);
        f_color_blendSet(F_COLOR_BLEND_ALPHA_75);

        f_draw_circle(coords.x,
                      coords.y,
                      f_fix_toInt(radius + f_fix_mul(radius / 4, sin12)));

        f_color_fillDrawSet(true);
        f_color_blendSet(F_COLOR_BLEND_ALPHA);

        f_color_colorSetHex(Orb->type->color2);
        f_color_alphaSet(F_COLOR_ALPHA_MAX / 4
                            + f_fix_toInt(sin12 * F_COLOR_ALPHA_MAX / 8));

        f_draw_circle(coords.x,
                      coords.y,
                      f_fix_toInt(radius + f_fix_mul(radius / 4, sin14)));

        f_color_colorSetHex(Orb->type->color1);
        f_color_alphaSet(F_COLOR_ALPHA_MAX * 6 / 8
                            + f_fix_toInt(sin14 * F_COLOR_ALPHA_MAX / 8));

        f_draw_circle(coords.x,
                      coords.y,
                      f_fix_toInt(radius / 2 + f_fix_mul(radius / 8, sin13)));
    }
}
