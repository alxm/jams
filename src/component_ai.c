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

#include "component_ai.h"

struct ZCompAi {
    AList* messageQueue;
    ZCompAiHandler* handler;
    void* context;
};

struct ZAiMessage {
    ZAiMessageType type;
    AEntity* relevantEntity;
};

size_t z_comp_ai_size(void)
{
    return sizeof(ZCompAi);
}

void z_comp_ai_init(ZCompAi* Ai, ZCompAiHandler* Handler, size_t ContextSize)
{
    Ai->messageQueue = a_list_new();
    Ai->handler = Handler;

    Ai->context = a_mem_malloc(ContextSize);
    memset(Ai->context, 0, ContextSize);
}

void z_comp_ai_free(void* Self)
{
    ZCompAi* ai = Self;

    A_LIST_ITERATE(ai->messageQueue, ZAiMessage*, m) {
        a_entity_release(m->relevantEntity);
        free(m);
    }

    a_list_free(ai->messageQueue);
    free(ai->context);
}

void z_comp_ai_runHandler(const ZCompAi* Ai, ZAiMessageType Type, AEntity* Relevant)
{
    Ai->handler(a_component_getEntity(Ai), Type, Relevant);
}

void* z_comp_ai_getContext(const ZCompAi* Ai)
{
    return Ai->context;
}

void z_comp_ai_queueMessage(ZCompAi* Ai, ZAiMessageType Type, AEntity* Relevant)
{
    ZAiMessage* m = a_mem_malloc(sizeof(ZAiMessage));

    m->type = Type;
    m->relevantEntity = Relevant;

    a_entity_reference(Relevant);

    a_list_addLast(Ai->messageQueue, m);
}

ZAiMessage* z_comp_ai_getMessage(const ZCompAi* Ai)
{
    ZAiMessage* m = a_list_pop(Ai->messageQueue);

    if(m) {
        a_entity_release(m->relevantEntity);
    }

    return m;
}

void z_comp_ai_getMessageData(const ZAiMessage* Message, ZAiMessageType* Type, AEntity** Relevant)
{
    *Type = Message->type;
    *Relevant = Message->relevantEntity;
}
