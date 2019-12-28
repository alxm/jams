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

typedef struct ZWorld ZWorld;

extern ZWorld* z_world_new(ZTileType* MapData, int MapWidth, int MapHeight);
extern void z_world_finalize(ZWorld* World, int PlayerX, int PlayerY);
extern void z_world_free(ZWorld* World);

extern AColMap* z_world_getColmap(const ZWorld* World);
extern AEntity* z_world_getMap(const ZWorld* World);
extern AEntity* z_world_getPlayer(const ZWorld* World);
extern AEntity* z_world_getCamera(const ZWorld* World);
extern void z_world_message(const ZWorld* World, const char* String1, const char* String2, unsigned int Ticks);
extern void z_world_clearMessage(const ZWorld* World);
