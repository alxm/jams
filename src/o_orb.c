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

static const OOrbType g_types[O_ORB_TYPE_NUM] = {
    [O_ORB_TYPE_PLAYER] = {F_FIX_ONE * 1 / 4, {0xff, 0, 0}, NULL},
    [O_ORB_TYPE_NPC1] = {F_FIX_ONE * 2 / 4, {0, 0xff, 0}, NULL},
    [O_ORB_TYPE_NPC2] = {F_FIX_ONE * 3 / 4, {0, 0, 0xff}, NULL},
};

OOrb* o_orb_new(OOrbTypeId Type, FFix X, FFix Y)
{
    OOrb* o = f_mem_malloc(sizeof(OOrb));

    o->type = &g_types[Type];
    o->coords.x = X;
    o->coords.y = Y;

    return o;
}

void o_orb_free(OOrb* Orb)
{
    f_mem_free(Orb);
}

void o_orb_tick(OOrb* Orb)
{
    F_UNUSED(Orb);
}

void o_orb_draw(OOrb* Orb)
{
    FVecInt coords = n_cam_coordsToScreen(Orb->coords);

    f_color_blendSet(F_COLOR_BLEND_ALPHA_25);
    f_color_colorSetRgb(
        Orb->type->color.r, Orb->type->color.g, Orb->type->color.b);

    f_draw_circle(
        coords.x, coords.y, f_fix_toInt(Orb->type->radius * N_CAM_SCALE));
}
