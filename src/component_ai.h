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

typedef struct ZCompAi ZCompAi;

typedef enum {
    Z_AI_MESSAGE_BENEVOLENT,
    Z_AI_MESSAGE_AGGRESSIVE,
} ZAiMessageType;

typedef void ZAiMessageHandler(AEntity* Entity, ZCompAi* Ai, ZAiMessageType Type, AEntity* Relevant);
typedef void ZAiTickHandler(AEntity* Entity, ZCompAi* Ai);

extern size_t z_comp_ai_size(void);
extern void z_comp_ai_init(ZCompAi* Ai, ZAiMessageHandler* MessageHandler, ZAiTickHandler* TickHandler, size_t ContextSize);
extern AComponentFree z_comp_ai_free;

extern void z_comp_ai_runMessageHandler(ZCompAi* Ai, ZAiMessageType Type, AEntity* Relevant);
extern void z_comp_ai_runTickHandler(ZCompAi* Ai);
extern void* z_comp_ai_getContext(const ZCompAi* Ai);

extern void z_comp_ai_queueMessage(ZCompAi* Ai, ZAiMessageType Type, AEntity* Relevant);
extern bool z_comp_ai_getMessage(ZCompAi* Ai, ZAiMessageType* Type, AEntity** Relevant);
