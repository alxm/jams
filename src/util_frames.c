/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

static AStrHash* g_frames; // table of ASpriteFrames

static void loadFrames(const char* Id, const ASprite* Sheet, int X, int Y, unsigned Ms)
{
    ASpriteFrames* frames = a_spriteframes_new(Sheet,
                                               X,
                                               Y,
                                               a_fps_msToFrames(Ms));

    a_strhash_add(g_frames, Id, frames);
}

static void freeFrames(ASpriteFrames* Frames)
{
    a_spriteframes_free(Frames, true);
}

void z_util_frames_load(void)
{
    g_frames = a_strhash_new();

    ASprite* sh = a_sprite_newFromFile("gfx/sprites.png");

    loadFrames("buildingBase", sh, 0, 17, 0);
    loadFrames("buildingDepot", sh, 0, 34, 0);
    loadFrames("buildingTurret", sh, 0, 51, 0);
    loadFrames("crystal", sh, 0, 0, 100);
    loadFrames("cursor", sh, 36, 68, 0);
    loadFrames("cursorUnitHover", sh, 51, 68, 200);
    loadFrames("cursorUnitSelected", sh, 66, 68, 200);
    loadFrames("cursorBuildingHover", sh, 117, 68, 200);
    loadFrames("cursorBuildingSelected", sh, 140, 68, 200);
    loadFrames("workerUp", sh, 0, 68, 100);
    loadFrames("workerDown", sh, 0, 77, 100);
    loadFrames("workerLeft", sh, 0, 86, 100);
    loadFrames("workerRight", sh, 0, 95, 100);

    a_sprite_free(sh);
}

void z_util_frames_unload(void)
{
    a_strhash_freeEx(g_frames, (AFree*)freeFrames);
}

ASpriteFrames* z_util_frames_get(const char* Id)
{
    return a_strhash_get(g_frames, Id);
}

ASpriteFrames* z_util_frames_dup(const char* Id)
{
    return a_spriteframes_dup(a_strhash_get(g_frames, Id), false);
}
