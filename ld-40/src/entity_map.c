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

#include "util_level.h"
#include "util_terrain.h"

#include "component_mapbuildings.h"
#include "component_mapgfx.h"
#include "component_mapterrain.h"

AEntity* z_entity_map_new(ZStateGame* Game, const ZUtilLevel* Level)
{
    AEntity* e = a_entity_new("map", Game);

    ZCompMapBuildings* buildings = a_entity_addComponent(e, "mapBuildings");
    z_comp_mapbuildings_init(buildings, Level);

    ZCompMapTerrain* terrain = a_entity_addComponent(e, "mapTerrain");
    z_comp_mapterrain_init(terrain, Level);

    ZCompMapGfx* gfx = a_entity_addComponent(e, "mapGfx");
    z_comp_mapgfx_init(gfx, z_comp_mapterrain_getMap(terrain), Level);

    return e;
}
