/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Mine Op 40 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mine Op 40 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mine Op 40.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "state_game.h"

#include "entity_map.h"

struct ZStateGame {
    AEntity* map;
};

static void initGame(ZStateGame* Game)
{
    ASprite* image = a_sprite_newFromFile("gfx/map0.png");
    APixel* data = a_sprite_getPixels(image);
    int dataW = a_sprite_getWidth(image);
    int dataH = a_sprite_getHeight(image);

    Game->map = z_entity_map_new(Game, data, dataW, dataH);
}

static void freeGame(ZStateGame* Game)
{
    A_UNUSED(Game);
}

A_STATE(game)
{
    static ZStateGame game;

    A_STATE_INIT
    {
        initGame(&game);
    }

    A_STATE_FREE
    {
        freeGame(&game);
    }
}
