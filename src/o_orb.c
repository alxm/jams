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

#define ORB_SPEED (F_FIX_ONE / 16)

static void h_orb_player(OOrb* Orb)
{
    if(f_button_pressGet(u_input_get(U_BUTTON_UP))) {
        Orb->coords.y -= ORB_SPEED;
    }

    if(f_button_pressGet(u_input_get(U_BUTTON_DOWN))) {
        Orb->coords.y += ORB_SPEED;
    }

    if(f_button_pressGet(u_input_get(U_BUTTON_LEFT))) {
        Orb->coords.x -= ORB_SPEED;
    }

    if(f_button_pressGet(u_input_get(U_BUTTON_RIGHT))) {
        Orb->coords.x += ORB_SPEED;
    }
}

static void h_orb_npc(OOrb* Orb)
{
    Orb->coords.x += f_random_range(-ORB_SPEED/4, ORB_SPEED/4);
    Orb->coords.y += f_random_range(-ORB_SPEED/4, ORB_SPEED/4);
}

static const OOrbType g_types[O_ORB_TYPE_NUM] = {
    [O_ORB_TYPE_PLAYER] = {F_FIX_ONE * 1 / 4, {0xff, 0, 0}, h_orb_player},
    [O_ORB_TYPE_NPC1] = {F_FIX_ONE * 2 / 4, {0, 0xff, 0}, h_orb_npc},
    [O_ORB_TYPE_NPC2] = {F_FIX_ONE * 3 / 4, {0, 0, 0xff}, h_orb_npc},
};

OOrb* o_orb_new(OOrbTypeId Type, FFix X, FFix Y)
{
    OOrb* o = f_mem_malloc(sizeof(OOrb));

    o->type = &g_types[Type];
    o->coords.x = X;
    o->coords.y = Y;
    o->offset = f_random_intu(F_FIX_ANGLES_NUM);

    return o;
}

void o_orb_free(OOrb* Orb)
{
    f_mem_free(Orb);
}

void o_orb_tick(OOrb* Orb)
{
    Orb->type->tick(Orb);
}

void o_orb_draw(OOrb* Orb)
{
    FVecInt coords = n_cam_coordsToScreen(Orb->coords);

    f_color_blendSet(F_COLOR_BLEND_ALPHA_25);
    f_color_colorSetRgb(
        Orb->type->color.r, Orb->type->color.g, Orb->type->color.b);

    f_draw_circle(
        coords.x,
        coords.y,
        f_fix_toInt(Orb->type->radius * N_CAM_SCALE
                        + f_fix_mul(Orb->type->radius * N_CAM_SCALE / 4,
                                    f_fps_ticksSin(1, 1, Orb->offset))));
}
