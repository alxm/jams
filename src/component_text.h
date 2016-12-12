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

typedef struct ZCompText ZCompText;

extern size_t z_comp_text_size(void);
extern void z_comp_text_init(ZCompText* Text);

extern void z_comp_text_getStrings(const ZCompText* Text, const char** String1, const char** String2);
extern void z_comp_text_setStrings(ZCompText* Text, const char* String1, const char* String2);
extern unsigned int z_comp_text_getTtl(const ZCompText* Text);
extern void z_comp_text_setTtl(ZCompText* Text, unsigned int Ticks);
