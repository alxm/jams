#include <a2x.h>

typedef struct ZCompPosition {
    AFix x, y;
} ZCompPosition;

size_t z_comp_position_size(void)
{
    return sizeof(ZCompPosition);
}
