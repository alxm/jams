/*
    Copyright 2016 Alex Margarit

    This file is part of Motel 37.

    Motel 37 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Motel 37 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Motel 37.  If not, see <http://www.gnu.org/licenses/>.
*/

typedef struct ZCompMap ZCompMap;

extern size_t z_comp_map_size(void);
extern AComponentFree z_comp_map_free;
extern void z_comp_map_init(ZCompMap* Map, ZTileType* MapData, int MapWidth, int MapHeight);

extern ASprite* z_comp_map_sprite(const ZCompMap* Map, int X, int Y);
extern void z_comp_map_getDim(const ZCompMap* Map, int* W, int* H);
extern bool z_comp_map_canWalk(const ZCompMap* Map, int X, int Y);
