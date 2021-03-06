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

typedef enum {
    Z_MOVE_UP,
    Z_MOVE_DOWN,
    Z_MOVE_LEFT,
    Z_MOVE_RIGHT
} ZMove;

extern AEntity* z_entity_macro_spawn(ZCompMap* Map, const char* Name, const char* Up, const char* Down, const char* Left, const char* Right);
extern bool z_entity_macro_move(AEntity* Entity, ZMove Direction);

extern AMessageHandler z_entity_macro_handleGreeting;
extern AMessageHandler z_entity_macro_handleAttack;
