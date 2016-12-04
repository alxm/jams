#include <a2x.h>

#include "component_position.h"

struct ZCompPosition {
    int x, y;
    int dirX, dirY;
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
    Position->dirX = 0;
    Position->dirY = 0;
    Position->anchor = NULL;
}

void z_comp_position_initPointer(ZCompPosition* Position, ZCompPosition* Anchor)
{
    z_comp_position_init(Position, 0, 0);
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

void z_comp_position_move(ZCompPosition* Position, int DirX, int DirY)
{
    Position->dirX = DirX;
    Position->dirY = DirY;
}
