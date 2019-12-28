/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    38th Century - A simple world simulation game made for Ludum Dare 38 72h

    38th Century is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    38th Century is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 38th Century.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef struct ZControls ZControls;

struct ZControls {
    AInputButton* up;
    AInputButton* down;
    AInputButton* left;
    AInputButton* right;
    AInputButton* primary;
    AInputButton* secondary;
    AInputButton* start;
};

extern ZControls z_controls;

extern void z_controls_load(void);
extern void z_controls_release(void);
