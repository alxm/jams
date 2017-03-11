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

typedef struct ZCompInteract ZCompInteract;

typedef enum {
    Z_INTERACTION_BENEVOLENT,
    Z_INTERACTION_AGGRESSIVE
} ZInteractionType;

extern size_t z_comp_interact_size(void);
extern void z_comp_interact_init(ZCompInteract* Interact, const char* Name);
extern AComponentFree z_comp_interact_free;

extern void z_comp_interact_action(ZCompInteract* Interact, AEntity* Actor, ZInteractionType Type);
extern const char* z_comp_interact_getName(const ZCompInteract* Interact);

extern bool z_comp_interact_getPending(ZCompInteract* Interact, AEntity** Actor, ZInteractionType* Type);
