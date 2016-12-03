#include <a2x.h>

typedef struct ZCompPosition {
    int x, y;
    struct ZCompPosition* anchor;
} ZCompPosition;

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
    Position->x = Anchor->x;
    Position->y = Anchor->y;
    Position->anchor = Anchor;
}
