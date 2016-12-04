#include <a2x.h>

#include "component_cathedral.h"
#include "component_position.h"
#include "graphics.h"
#include "world.h"

void z_system_cathedralTick(AEntity* Entity, void* GlobalContext)
{
    z_comp_cathedral_incAngle(a_entity_getComponent(Entity, "cathedral"));
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
    ASprite* colors = z_graphics_getStill("circleColors");
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
