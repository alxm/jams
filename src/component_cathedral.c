#include <a2x.h>

#include "component_cathedral.h"
#include "graphics.h"

struct ZCompCathedral {
    AList* circles;
};

size_t z_comp_cathedral_size(void)
{
    return sizeof(ZCompCathedral);
}

void z_comp_cathedral_free(void* Self)
{
    ZCompCathedral* cathedral = Self;

    A_LIST_ITERATE(cathedral->circles, ASprite*, sprite) {
        a_sprite_free(sprite);
    }

    a_list_free(cathedral->circles);
}

void z_comp_cathedral_init(ZCompCathedral* Cathedral)
{
    Cathedral->circles = a_list_new();

    ASprite* mask = z_graphics_getStill("colorcircle");
    const int maskW = a_sprite_w(mask);
    const int maskH = a_sprite_h(mask);

    for(int i = 0; i < 8; i++) {
        ASprite* s = a_sprite_blank(maskW, maskH, false);
        a_list_addLast(Cathedral->circles, s);

        a_screen_setTargetSprite(s);
        APixel* pixels = a_screen_pixels();

        for(int i = maskW * maskH; i--; ) {
            *pixels++ = a_pixel_make(a_random_int(256),
                                     a_random_int(256),
                                     a_random_int(256));
        }

        a_sprite_blit(mask, 0, 0);
        a_screen_resetTarget();
    }
}

ASprite* z_comp_cathedral_getGlass(const ZCompCathedral* Cathedral)
{
    return a_list_get(Cathedral->circles,
                      a_random_int(a_list_size(Cathedral->circles)));
}
