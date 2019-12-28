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

#include "util_bar.h"
#include "util_colors.h"

#include "component_accosted.h"
#include "component_motion.h"
#include "component_position.h"
#include "component_sprite.h"

void z_system_sprite(AEntity* Entity)
{
    ZCompAccosted* accosted = a_entity_getComponent(Entity, "accosted");
    ZCompMotion* motion = a_entity_getComponent(Entity, "motion");
    ZCompPosition* position = a_entity_requireComponent(Entity, "position");
    ZCompSprite* sprite = a_entity_requireComponent(Entity, "sprite");

    ASprite* graphic;

    if(motion
        && z_comp_motion_getState(motion) == Z_COMP_MOTION_STATE_RESTING) {

        graphic = z_comp_sprite_getGraphic0(sprite);
    } else {
        graphic = z_comp_sprite_getGraphic(sprite);
    }

    AFix x, y;
    z_comp_position_getCoords(position, &x, &y);

    int originX, originY;
    z_state_game_getOrigin(a_entity_getContext(Entity), &originX, &originY);

    int screenOriginX = a_screen_getWidth() / 2;
    int screenOriginY = a_screen_getHeight() / 2;

    int width = a_sprite_getWidth(graphic);
    int height = a_sprite_getHeight(graphic);
    int drawX = screenOriginX + a_fix_fixtoi(x) - originX;
    int drawY = screenOriginY + a_fix_fixtoi(y) - originY;

    a_sprite_blit(graphic, drawX - width / 2, drawY - height / 2);

    if(accosted) {
        z_util_bar_draw(z_util_colors.red2,
                        z_util_colors.blue2,
                        z_comp_accosted_getConvinced(accosted),
                        100,
                        drawX - width / 4,
                        drawY - height / 2 - 2,
                        width / 2,
                        2);
    }
}

int z_system_sprite_sort(AEntity* A, AEntity* B)
{
    ZCompSprite* spriteA = a_entity_requireComponent(A, "sprite");
    ZCompSprite* spriteB = a_entity_requireComponent(B, "sprite");

    ZCompSpriteLayer layerA = z_comp_sprite_getLayer(spriteA);
    ZCompSpriteLayer layerB = z_comp_sprite_getLayer(spriteB);

    if(layerA != layerB) {
        return layerA - layerB;
    }

    ZCompPosition* positionA = a_entity_requireComponent(A, "position");
    ZCompPosition* positionB = a_entity_requireComponent(B, "position");

    AFix xA, yA;
    z_comp_position_getCoords(positionA, &xA, &yA);

    AFix xB, yB;
    z_comp_position_getCoords(positionB, &xB, &yB);

    return yA - yB;
}
