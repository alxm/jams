/*
    Copyright 2016 Alex Margarit

    This file is part of Motel 37.

    Motel 37 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Motel 37 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Motel 37.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_text.h"

void z_system_messageBoxTick(AEntity* Entity, void* GlobalContext)
{
    ZCompText* text = a_entity_getComponent(Entity, "text");
    unsigned int ttl = z_comp_text_getTtl(text);

    if(ttl > 0) {
        if(--ttl == 0) {
            z_comp_text_setStrings(text, "", "");
        }

        z_comp_text_setTtl(text, ttl);
    }
}

void z_system_messageBoxDraw(AEntity* Entity, void* GlobalContext)
{
    ZCompText* text = a_entity_getComponent(Entity, "text");

    const char *s1, *s2;
    z_comp_text_getStrings(text, &s1, &s2);

    a_font_setAlign(A_FONT_ALIGN_LEFT);
    a_font_setFace(A_FONT_FACE_WHITE);
    a_font_setCoords(8, 216);

    a_font_text(s1);
    a_font_newLine();
    a_font_text(s2);
}
