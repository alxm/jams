/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    Space Station Plunder (SSP) - A roguelike game made for 7DRL 2017!

    SSP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSP.  If not, see <http://www.gnu.org/licenses/>.
*/

#define Z_MAP_TILES_W 10
#define Z_MAP_TILES_H 10
#define Z_MAP_TILE_DIM 24

#define Z_MAP_PIXEL_W (Z_MAP_TILES_W * Z_MAP_TILE_DIM)
#define Z_MAP_PIXEL_H (Z_MAP_TILES_H * Z_MAP_TILE_DIM)

typedef struct ZGraphic ZGraphic;

extern void z_graphics_load(void);
extern void z_graphics_unload(void);

extern const ZGraphic* z_graphics_get(const char* Name);

extern unsigned z_graphics_numFrames(const ZGraphic* Graphic);
extern ASprite* z_graphics_getFrame(const ZGraphic* Graphic, unsigned Frame);
