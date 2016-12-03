#pragma once

extern void z_map_tile_load(void);

extern size_t z_map_comp_size(void);
extern AEntity* z_map_new(void);
extern AComponentFree z_map_comp_free;

extern ASystemHandler z_map_comp_handler_draw;
