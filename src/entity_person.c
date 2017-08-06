/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "state_game.h"

#include "util_controls.h"
#include "util_tiles.h"

#include "component_ai.h"
#include "component_position.h"
#include "component_sprite.h"
#include "component_volume.h"

typedef struct ZPersonAiContext {
    int tileGoalX, tileGoalY;
} ZPersonAiContext;

static void person_walkAroundAi(AEntity* Entity)
{
    A_UNUSED(Entity);
}

AEntity* z_entity_person_new(ZStateGame* Game, int TileX, int TileY)
{
    AEntity* e = a_entity_new("player", Game);

    AFix x = a_fix_itofix(TileX * Z_UTIL_TILE_DIM + Z_UTIL_TILE_DIM / 2);
    AFix y = a_fix_itofix(TileY * Z_UTIL_TILE_DIM + Z_UTIL_TILE_DIM / 2);

    ZCompAi* ai = a_entity_addComponent(e, "ai");
    z_comp_ai_init(ai, person_walkAroundAi, sizeof(ZPersonAiContext));
    ZPersonAiContext* ctx = z_comp_ai_getContext(ai);
    ctx->tileGoalX = -1;
    ctx->tileGoalY = -1;

    ZCompPosition* position = a_entity_addComponent(e, "position");
    z_comp_position_init(position, x, y);

    ZCompSprite* sprite = a_entity_addComponent(e, "sprite");
    z_comp_sprite_init(sprite,
                       "personUp",
                       "personDown",
                       "personLeft",
                       "personRight");

    ZCompVolume* volume = a_entity_addComponent(e, "volume");
    z_comp_volume_init(volume, z_state_game_getVolumeColMap(Game), x, y, 4);

    return e;
}
