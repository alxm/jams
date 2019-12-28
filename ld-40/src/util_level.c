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

#include "util_level.h"

struct ZUtilLevel {
    ASprite* sprite;
    int w, h;
    APixel* data;
};

ZUtilLevel* z_util_level_load(const char* Path)
{
    ZUtilLevel* level = a_mem_malloc(sizeof(ZUtilLevel));
    ASprite* image = a_sprite_newFromFile(Path);

    level->sprite = image;
    level->w = a_sprite_getWidth(image);
    level->h = a_sprite_getHeight(image);
    level->data = a_sprite_getPixels(image);

    return level;
}

void z_util_level_free(ZUtilLevel* Level)
{
    a_sprite_free(Level->sprite);
    free(Level);
}

void z_util_level_getDim(const ZUtilLevel* Level, int* W, int* H)
{
    *W = Level->w;
    *H = Level->h;
}

APixel z_util_level_getValue(const ZUtilLevel* Level, int X, int Y)
{
    return Level->data[Y * Level->w + X];
}
