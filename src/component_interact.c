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

struct ZPendingAction {
    AEntity* actor;
    ZActionType action;
};

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

void z_comp_interact_action(ZCompInteract* Interact, AEntity* Actor, ZActionType Action)
{
    ZPendingAction* pending = a_mem_malloc(sizeof(ZPendingAction));

    pending->actor = Actor;
    pending->action = Action;

    a_list_addLast(Interact->pending, pending);
    z_game_setLogAction("Attacking %s", Interact->name);
}

const char* z_comp_interact_getName(const ZCompInteract* Interact)
{
    return Interact->name;
}

AList* z_comp_interact_getPending(const ZCompInteract* Interact)
{
    return Interact->pending;
}

void z_comp_interact_getActionData(const ZPendingAction* Action, AEntity** Actor, ZActionType* ActionType)
{
    *Actor = Action->actor;
    *ActionType = Action->action;
}
