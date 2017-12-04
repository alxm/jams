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

#include "util_frames.h"

#include "component_cursor.h"

struct ZCompCursor {
    AEntity* unitHover; // unit that cursor is hovering over
    AEntity* unitSelected; // unit that cursor selected
    ASpriteFrames* light[Z_COMP_CURSOR_NUM];
};

size_t z_comp_cursor_size(void)
{
    return sizeof(ZCompCursor);
}

void z_comp_cursor_init(ZCompCursor* Cursor)
{
    Cursor->unitHover = NULL;
    Cursor->unitSelected = NULL;

    Cursor->light[Z_COMP_CURSOR_UNIT_HOVER]
        = z_util_frames_get("cursorUnitHover");

    Cursor->light[Z_COMP_CURSOR_UNIT_SELECTED]
        = z_util_frames_get("cursorUnitSelected");
}

void z_comp_cursor_free(void* Self)
{
    ZCompCursor* cursor = Self;

    if(cursor->unitHover != NULL) {
        a_entity_release(cursor->unitHover);
    }

    if(cursor->unitSelected != NULL) {
        a_entity_release(cursor->unitSelected);
    }
}

AEntity* z_comp_cursor_getHover(const ZCompCursor* Cursor)
{
    return Cursor->unitHover;
}

void z_comp_cursor_setHover(ZCompCursor* Cursor, AEntity* Unit)
{
    if(Cursor->unitHover != NULL) {
        a_entity_release(Cursor->unitHover);
    }

    Cursor->unitHover = Unit;

    if(Unit != NULL) {
        a_entity_reference(Unit);
    }
}

AEntity* z_comp_cursor_getSelected(const ZCompCursor* Cursor)
{
    return Cursor->unitSelected;
}

void z_comp_cursor_setSelected(ZCompCursor* Cursor, AEntity* Unit)
{
    if(Cursor->unitSelected != NULL) {
        a_entity_release(Cursor->unitSelected);
    }

    Cursor->unitSelected = Unit;

    if(Unit != NULL) {
        a_entity_reference(Unit);
        a_spriteframes_reset(Cursor->light[Z_COMP_CURSOR_UNIT_SELECTED]);
    }
}

void z_comp_cursor_lightTick(const ZCompCursor* Cursor)
{
    for(ZCompCursorType t = 0; t < Z_COMP_CURSOR_NUM; t++) {
        a_spriteframes_next(Cursor->light[t]);
    }

    if(Cursor->unitSelected == NULL) {
        a_spriteframes_reset(Cursor->light[Z_COMP_CURSOR_UNIT_SELECTED]);
    }
}

ASprite* z_comp_cursor_getLight(const ZCompCursor* Cursor, ZCompCursorType Type)
{
    return a_spriteframes_getCurrent(Cursor->light[Type]);
}
