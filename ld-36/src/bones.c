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

#include "bones.h"

typedef struct Bone {
    ASprite* sprite;
    AFix x;
    int y;
} Bone;

struct BoneBits {
    LandInstance* land;
    AList* bonesList;
    AList* grassList;
};

BoneBits* bones_new(LandInstance* Land)
{
    BoneBits* bones = a_mem_malloc(sizeof(BoneBits));

    bones->land = Land;
    bones->bonesList = a_list_new();
    bones->grassList = a_list_new();

    for(int i = a_fix_fixtoi(a_fix_div(a_fix_itofix(a_screen_width()), land_getSpeed(Land))); i--; ) {
        land_tick(Land);
        bones_tick(bones);
    }

    return bones;
}

void bones_free(BoneBits* Bones)
{
    A_LIST_ITERATE(Bones->bonesList, Bone*, b) {
        free(b);
    }

    A_LIST_ITERATE(Bones->grassList, Bone*, b) {
        free(b);
    }

    a_list_free(Bones->bonesList);
    a_list_free(Bones->grassList);

    free(Bones);
}

void bones_tick(BoneBits* Bones)
{
    A_LIST_ITERATE(Bones->bonesList, Bone*, b) {
        b->x -= land_getSpeed(Bones->land);

        if(a_fix_fixtoi(b->x) + a_sprite_w(b->sprite) < land_getBufferOffset(Bones->land)) {
            free(b);
            A_LIST_REMOVE_CURRENT();
        }
    }

    A_LIST_ITERATE(Bones->grassList, Bone*, b) {
        b->x -= land_getSpeed(Bones->land);

        if(a_fix_fixtoi(b->x) + a_sprite_w(b->sprite) < land_getBufferOffset(Bones->land)) {
            free(b);
            A_LIST_REMOVE_CURRENT();
        }
    }

    if(a_random_int(4) == 0) {
        int x = land_getBufferOffset(Bones->land) + a_screen_width();
        Bone* b = a_mem_malloc(sizeof(Bone));

        b->sprite = a_spriteframes_getRandom(g_data.boneGraphics);
        b->x = a_fix_itofix(x);

        int topY = land_getTopY(Bones->land, x + a_sprite_w(b->sprite) / 2);
        b->y = topY + a_random_int(a_screen_height() - topY);

        a_list_addLast(Bones->bonesList, b);
    }

    if(a_random_int(16) == 0) {
        int x = land_getBufferOffset(Bones->land) + a_screen_width();
        Bone* b = a_mem_malloc(sizeof(Bone));

        b->sprite = a_spriteframes_getRandom(g_data.grassGraphics);
        b->x = a_fix_itofix(x);

        int topY = land_getTopY(Bones->land, x + a_sprite_w(b->sprite) / 2);
        b->y = topY - a_sprite_h(b->sprite) + 1;

        a_list_addLast(Bones->grassList, b);
    }
}

void bones_draw(BoneBits* Bones)
{
    A_LIST_ITERATE(Bones->bonesList, Bone*, b) {
        a_blit(b->sprite, a_fix_fixtoi(b->x) - land_getBufferOffset(Bones->land), b->y);
    }

    A_LIST_ITERATE(Bones->grassList, Bone*, b) {
        a_blit(b->sprite, a_fix_fixtoi(b->x) - land_getBufferOffset(Bones->land), b->y);
    }
}
