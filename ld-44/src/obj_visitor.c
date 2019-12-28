/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of Coffin Digital, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "obj_visitor.h"

#include "util_gfx.h"

typedef struct {
    UGfxId face;
} NVisitor;

static const char* g_names[] = {
    "Raylee Scott",
    "Blair Johnson",
    "Riley Day",
    "Jessie Dean",
    "Jessie Walsh",
    "Gene Adkins",
    "Clem Tucker",
    "Taylor Leach",
    "Steff Daugherty",
    "Harley Hubbard",
};

static NVisitor g_visitors[N_VISITOR_NUM] = {
    [N_VISITOR_0] = {U_GFX_FACE_0},
    [N_VISITOR_1] = {U_GFX_FACE_1},
    [N_VISITOR_2] = {U_GFX_FACE_2},
};

static NVisitorId g_id = N_VISITOR_INVALID;
static const char* g_name;

void n_visitor_new(void)
{
    g_id = a_random_int(N_VISITOR_NUM);
    g_name = g_names[a_random_intu(A_ARRAY_LEN(g_names))];
}

void n_visitor_tick(void)
{
    if(g_id == N_VISITOR_INVALID) {
        return;
    }

    //
}

void n_visitor_draw(void)
{
    if(g_id == N_VISITOR_INVALID) {
        return;
    }

    const NVisitor* v = &g_visitors[g_id];

    a_sprite_blit(u_gfx_getNext(v->face), 73, 83);
}

const char* n_visitor_nameGet(void)
{
    return g_name;
}
