/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Cave Shrine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cave Shrine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cave Shrine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "entity_enemy.h"

#include "macro_move.h"

#include "util_ecs.h"

AEntity* e_enemy_new(TGame* Game, TGameEntityContext* Context)
{
    AEntity* e = a_entity_newEx(Context->template, Context, Game);

    a_entity_messageSet(e, U_MSG_INTERACT, m_move_bumpHandler);
    m_move_coordsSet(e, Context->coords);

    return e;
}
