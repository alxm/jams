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

#include "component_cathedral.h"
#include "component_position.h"
#include "component_sprite.h"
#include "graphics.h"
#include "tiles.h"
#include "world.h"

void z_system_cathedralTick(AEntity* Entity, void* GlobalContext)
{
    AEntity* player = z_world_getPlayer(GlobalContext);
    ZCompPosition* playerPosition = a_entity_getComponent(player, "position");
    ZCompSprite* playerSprite = a_entity_getComponent(player, "sprite");
    ZCompCathedral* cathedral = a_entity_getComponent(Entity, "cathedral");

    int playerX, playerY;
    z_comp_position_getCoords(playerPosition, &playerX, &playerY);
    playerX /= Z_TILE_DIM;
    playerY /= Z_TILE_DIM;

    z_comp_cathedral_setColors(cathedral,
                               playerX >= 7 && playerX <= 8 && playerY == 2
                               && z_comp_sprite_getDir(playerSprite) == Z_COMP_SPRITE_DIRECTION_UP);

    z_comp_cathedral_incAngle(cathedral);
}

void z_system_cathedralDraw(AEntity* Entity, void* GlobalContext)
{
    AEntity* camera = z_world_getCamera(GlobalContext);
    ZCompPosition* cameraPosition = a_entity_getComponent(camera, "position");
    ZCompPosition* spritePosition = a_entity_getComponent(Entity, "position");
    ZCompCathedral* cathedral = a_entity_getComponent(Entity, "cathedral");

    int camX, camY;
    z_comp_position_getCoords(cameraPosition, &camX, &camY);

    int sprX, sprY;
    z_comp_position_getCoords(spritePosition, &sprX, &sprY);

    ASprite* mask = z_graphics_getStill("circleFrame");
    ASprite* colors = z_comp_cathedral_getColors(cathedral);
    const int maskW = a_sprite_w(mask);
    const int maskH = a_sprite_h(mask);

    ASprite* glass = a_sprite_blank(maskW, maskH, false);
    a_screen_setTargetSprite(glass);

    uint8_t startAngle = z_comp_cathedral_getAngle(cathedral);

    AFix offsetX = 0;
    uint8_t angleX = startAngle + 64;

    for(unsigned int y = 0; y < maskH; y++) {
        AFix offsetY = 0;
        uint8_t angleY = startAngle;

        for(unsigned int x = 0; x < maskW; x++) {
            APixel p = a_sprite_getPixel(colors,
                                         (x + a_fix_fixtoi(offsetX)) % maskW,
                                         (y + a_fix_fixtoi(offsetY)) % maskH);
            a_pixel_setPixel(p);
            a_draw_pixel(x, y);

            angleY = startAngle + x * 256 / maskW;
            offsetY += a_fix_sin(angleY);
        }

        angleX = startAngle + 64 + y * 256 / maskH;
        offsetX += a_fix_cos(angleX);
    }

    a_sprite_blit(mask, 0, 0);
    a_screen_resetTarget();
    a_sprite_blit(glass,
                  a_screen_width() / 2 + (sprX - camX) - a_sprite_w(glass) / 2,
                  a_screen_height() / 2 + (sprY - camY));
    a_sprite_free(glass);
}
