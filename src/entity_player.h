#pragma once

extern size_t z_player_comp_size(void);
extern AEntity* z_player_new(void);
extern AComponentFree z_player_comp_free;

extern ASystemHandler z_player_comp_handler_tick;
extern ASystemHandler z_player_comp_handler_draw;
