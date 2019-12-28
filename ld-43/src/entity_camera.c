/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Neonrattle is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Neonrattle is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Neonrattle.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "entity_camera.h"

#include "component_position.h"

#include "util_ecs.h"

AEntity* e_camera_new(TGame* Game)
{
    AEntity* e = a_entity_new("camera", Game);

    a_entity_componentAdd(e, U_COM_CAMERA);
    a_entity_componentAdd(e, U_COM_POSITION);

    a_entity_activeSetPermanent(e);

    return e;
}

AVectorInt e_camera_originGet(AEntity* Camera)
{
    return c_position_coordsGet(a_entity_componentReq(Camera, U_COM_POSITION));
}

AVectorInt e_camera_worldToScreen(AEntity* Camera, AVectorInt World)
{
    AVectorInt o = e_camera_originGet(Camera);

    return (AVectorInt){a_screen_widthGet() / 2 + World.x - o.x,
                        a_screen_heightGet() / 2 + World.y - o.y};
}
