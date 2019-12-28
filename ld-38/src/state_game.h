/*
    Copyright 2017 Alex Margarit <http://www.alxm.org/>
    38th Century - A simple world simulation game made for Ludum Dare 38 72h

    38th Century is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    38th Century is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 38th Century.  If not, see <http://www.gnu.org/licenses/>.
*/

#define Z_TILE_WIDTH 20

#define Z_HISCORE_FILE "38thCentury.hiscore"
#define Z_HISCORE_KEY "38thCentury10"

typedef struct ZTerrain ZTerrain;

typedef struct ZGame {
    ZLog* log;
    ZData* data;
    ZPeriodType period;
    ZDisaster disaster;
    ZTerrain* tiles;
    unsigned tilesNum;
    int tilesBaseline;
    AList* structures; // list of ZStructure
    AMenu* menu;
    int evolutions;
    uint32_t hiscore;
} ZGame;

extern ZGame z_game;

extern void z_game_log(unsigned Font, const char* Format, ...);

extern bool z_game_kill(AFix Number);

extern bool z_game_tick(void);
extern void z_game_draw(ZHelpScreen HelpScreen);

extern A_STATE(play);
extern A_STATE(over);
extern A_STATE(hiscore);
extern A_STATE(evolution);
