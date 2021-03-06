/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef struct ZCompMood ZCompMood;

typedef enum {
    Z_MOOD_GOOD,
    Z_MOOD_EVIL,
} ZMoodType;

extern size_t z_comp_mood_size(void);
extern void z_comp_mood_init(ZCompMood* Mood, ZMoodType Type);

extern void z_comp_mood_setType(ZCompMood* Mood, ZMoodType Type);
extern ZMoodType z_comp_mood_getType(const ZCompMood* Mood);
