/*
    Copyright 2016 Alex Margarit

    This file is part of Church 2, a game prototype made for the
    Cool Jams Inc 48h game jam that ran from 2016-12-02 to 2016-12-04.

    Church 2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Church 2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Church 2.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "component_position.h"
#include "component_sprite.h"
#include "component_volume.h"

AEntity* z_entity_chest_new(int X, int Y, AColMap* Colmap)
{
    AEntity* chest = a_entity_new();

    z_comp_position_init(a_entity_addComponent(chest, "position"), X, Y);
    z_comp_volume_init(a_entity_addComponent(chest, "volume"), 8, Colmap, X, Y);
    z_comp_sprite_init(a_entity_addComponent(chest, "sprite"), "chest");

    return chest;
}
