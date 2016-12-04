#include <a2x.h>

#include "component_position.h"

struct ZCompPosition {
    int x, y;
    struct ZCompPosition* anchor;
};

size_t z_comp_position_size(void)
{
    return sizeof(ZCompPosition);
}

void z_comp_position_init(ZCompPosition* Position, int X, int Y)
{
    Position->x = X;
    Position->y = Y;
    Position->anchor = NULL;
}

void z_comp_position_initPointer(ZCompPosition* Position, ZCompPosition* Anchor)
{
    Position->x = 0;
    Position->y = 0;
    Position->anchor = Anchor;
}

void z_comp_position_getCoords(const ZCompPosition* Position, int* X, int* Y)
{
    if(Position->anchor == NULL) {
        *X = Position->x;
        *Y = Position->y;
    } else {
        *X = Position->anchor->x;
        *Y = Position->anchor->y;
    }
}
