/*
    Copyright 2016 Alex Margarit

    This file is part of Motel 37.

    Motel 37 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Motel 37 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Motel 37.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "tiles.h"
#include "world.h"
#include "missions.h"
#include "component_phone.h"
#include "component_sprite.h"

void z_system_phone(AEntity* Entity, void* GlobalContext)
{
    ZCompPhone* phone = a_entity_getComponent(Entity, "phone");
    ZCompSprite* sprite = a_entity_getComponent(Entity, "sprite");

    if(z_missions.canPhoneRing) {
        if(!z_comp_phone_isRinging(phone)) {
            z_comp_phone_setRinging(phone, true);
        }
    }

    if(z_comp_phone_isRinging(phone)) {
        z_comp_sprite_move(sprite);
        z_world_message(GlobalContext,
                        "Answer the phone!",
                        "",
                        0);
    } else {
        z_comp_sprite_stop(sprite);
    }
}
