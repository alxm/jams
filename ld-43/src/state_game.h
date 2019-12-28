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

#pragma once

#include <a2x.h>

typedef struct TGame TGame;

typedef struct {
    const char* template;
    AVectorInt coords;
} TGameEntityContext;

extern AState t_game;

extern AEntity* t_game_getPlayer(const TGame* Game);
extern AEntity* t_game_getCamera(const TGame* Game);
extern AEntity* t_game_getMap(const TGame* Game);

extern bool t_game_turnStart(const TGame* Game);
extern void t_game_runCode(TGame* Game, int Code);
