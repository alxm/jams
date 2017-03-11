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

#include <a2x.h>

#include "component_interact.h"

#include "state_game.h"

typedef struct ZPendingAction {
    AEntity* actor;
    ZInteractionType type;
} ZPendingAction;

struct ZCompInteract {
    char* name;
    AList* pending; // list of ZPendingAction
};

size_t z_comp_interact_size(void)
{
    return sizeof(ZCompInteract);
}

void z_comp_interact_init(ZCompInteract* Interact, const char* Name)
{
    Interact->name = a_str_dup(Name);
    Interact->pending = a_list_new();
}

void z_comp_interact_free(void* Self)
{
    ZCompInteract* interact = Self;

    A_LIST_ITERATE(interact->pending, ZPendingAction*, a) {
        free(a);
    }

    a_list_free(interact->pending);
    free(interact->name);
}

void z_comp_interact_action(ZCompInteract* Interact, AEntity* Actor, ZInteractionType Type)
{
    ZPendingAction* pending = a_mem_malloc(sizeof(ZPendingAction));

    pending->actor = Actor;
    pending->type = Type;

    a_list_addLast(Interact->pending, pending);
}

const char* z_comp_interact_getName(const ZCompInteract* Interact)
{
    return Interact->name;
}

bool z_comp_interact_getPending(ZCompInteract* Interact, AEntity** Actor, ZInteractionType* Type)
{
    ZPendingAction* p = a_list_pop(Interact->pending);

    if(p) {
        *Actor = p->actor;
        *Type = p->type;

        free(p);

        return true;
    }

    return false;
}
