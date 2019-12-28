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

#include "entity_map.h"

#include "component_input.h"
#include "component_position.h"

#include "macro_move.h"

#include "state_game.h"

#include "util_controls.h"
#include "util_ecs.h"

static void playerAction(AEntity* Player, UButtonId Button)
{
    if(Button >= U_BUTTON_UP && Button <= U_BUTTON_RIGHT
        && t_game_turnStart(a_entity_contextGet(Player))) {

        m_move_direction(Player, Button);
    }
}

AEntity* e_player_new(TGame* Game)
{
    AEntity* e = a_entity_newEx("player", NULL, Game);

    CInput* input = a_entity_componentAdd(e, U_COM_INPUT);
    c_input_bind(input, U_BUTTON_UP, playerAction);
    c_input_bind(input, U_BUTTON_DOWN, playerAction);
    c_input_bind(input, U_BUTTON_LEFT, playerAction);
    c_input_bind(input, U_BUTTON_RIGHT, playerAction);
    c_input_bind(input, U_BUTTON_ACTION, playerAction);

    CPosition* position = a_entity_componentAdd(e, U_COM_POSITION);
    c_position_directionSet(position, C_POSITION_DOWN);

    a_entity_activeSetPermanent(e);

    return e;
}
