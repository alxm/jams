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

#include <a2x.h>

#include "util_controls.h"
#include "util_data.h"
#include "util_graphics.h"
#include "util_log.h"
#include "util_period.h"

#include "state_game.h"

typedef enum {
    Z_MENU_DEFENSE,
    Z_MENU_HEALTH,
    Z_MENU_INDUSTRY,
    Z_MENU_SCIENCE,
    Z_MENU_NUM
} ZMenuItemType;

struct ZTerrain {
    int y;
};

typedef struct ZStructure {
    const ZGraphic* graphic;
    int tile;
} ZStructure;

ZGame z_game;

static void newStructure(const char* Id, int Tile)
{
    ZStructure* s = a_mem_malloc(sizeof(ZStructure));

    s->graphic = z_graphics_get(Id);
    s->tile = Tile;

    a_list_addLast(z_game.structures, s);
}

static void initGame(void)
{
    z_game.log = z_log_new(10);
    z_game.data = z_data_new();

    z_data_set(z_game.data, Z_DATA_YEAR, 0);
    z_data_set(z_game.data, Z_DATA_POPULATION, a_fix_itofix(32));
    z_data_set(z_game.data, Z_DATA_DEFENSE, 64);
    z_data_set(z_game.data, Z_DATA_HEALTH, 128);
    z_data_set(z_game.data, Z_DATA_INDUSTRY, 64);
    z_data_set(z_game.data, Z_DATA_SCIENCE, 0);

    z_game.period = Z_PERIOD_CAVES;
    z_game.disaster.type = a_random_int(Z_DISASTER_NUM);
    z_game.disaster.damage = 64;

    z_game.tilesNum = (unsigned)a_screen_width() / Z_TILE_WIDTH;
    z_game.tiles = a_mem_malloc(z_game.tilesNum * sizeof(ZTerrain));

    int baseLine = 120;
    int leeway = 2;
    int y = baseLine;

    z_game.tilesBaseline = baseLine + leeway;

    for(unsigned i = z_game.tilesNum; i--; ) {
        for(unsigned i = z_game.tilesNum; i--; ) {
            int dir = a_random_int(3) - 1;

            if(dir == -1) {
                if(y <= baseLine - leeway) {
                    dir = 0;
                }
            } else if(dir == 1) {
                if(y >= baseLine + leeway) {
                    dir = 0;
                }
            }

            y += dir;
            z_game.tiles[i].y = y;
        }
    }

    z_game.structures = a_list_new();
    newStructure(z_period_getId(z_game.period),
                 1 + a_random_int((int)z_game.tilesNum - 2));

    z_game.menu = a_menu_new(z_controls.down,
                             z_controls.up,
                             z_controls.primary,
                             NULL);

    char* menuItems[Z_MENU_NUM];
    menuItems[Z_MENU_DEFENSE] = "DEFENSE";
    menuItems[Z_MENU_HEALTH] = "HEALTH CARE";
    menuItems[Z_MENU_INDUSTRY] = "INDUSTRY";
    menuItems[Z_MENU_SCIENCE] = "SCIENCE";

    for(int i = 0; i < Z_MENU_NUM; i++) {
        a_menu_addItem(z_game.menu, menuItems[i]);
    }

    z_game.evolutions = 0;
    z_game.hiscore = 0;

    AFile* f = a_file_open(Z_HISCORE_FILE, "rb");

    if(f && a_file_checkPrefix(f, Z_HISCORE_KEY)) {
        a_file_read(f, &z_game.hiscore, sizeof(z_game.hiscore));
        a_file_close(f);
    }

    z_game_log(z_fonts.lightGray,
               "You start with %d people",
               a_fix_fixtoi(z_data_getReal(z_game.data, Z_DATA_POPULATION)));
    z_game_log(z_fonts.lightGray, "Good luck!");
}

static void freeGame(void)
{
    z_log_free(z_game.log);
    z_data_free(z_game.data);
    free(z_game.tiles);

    A_LIST_ITERATE(z_game.structures, ZStructure*, s) {
        free(s);
    }

    a_menu_free(z_game.menu);
}

static bool adjustPopulation(void)
{
    bool died = false;
    AFix newPopulation;
    AFix oldPopulation = z_data_getReal(z_game.data, Z_DATA_POPULATION);

    int health = z_data_getReal(z_game.data, Z_DATA_HEALTH);

    if(health < 64) {
        newPopulation = oldPopulation - oldPopulation / (4 + health / 8);
    } else {
        newPopulation = oldPopulation + oldPopulation / (health);
    }

    int popChange = a_fix_fixtoi(newPopulation) - a_fix_fixtoi(oldPopulation);

    if(popChange > 0) {
        z_game_log(z_fonts.lightGray,
                   "  Population grew from %d to %d",
                   a_fix_fixtoi(oldPopulation),
                   a_fix_fixtoi(newPopulation));

        // TODO: make buildings
    } else if(popChange < 0) {
        z_game_log(z_fonts.lightGray,
                   "  Population declined from %d to %d",
                   a_fix_fixtoi(oldPopulation),
                   a_fix_fixtoi(newPopulation));
        z_game_log(z_fonts.lightGray,
                   "    Invest more in %s",
                   z_data_getName(Z_DATA_HEALTH));
    }

    if(newPopulation < oldPopulation) {
        died = z_game_kill(oldPopulation - newPopulation);
    } else {
        z_data_set(z_game.data, Z_DATA_POPULATION, newPopulation);
    }

    return died;
}

static void disasterClue(void)
{
    const char* text[Z_DISASTER_NUM];
    text[Z_DISASTER_DISEASE] = "  You notice a lot of rats milling around";
    text[Z_DISASTER_CALAMITY] = "  You feel the ground shaking";
    text[Z_DISASTER_ATTACK] = "  You see a small unusual light in the sky";

    int r = a_random_int(10);

    if(r < 8) {
        ZDisasterType clue = r < 3
                             ? z_game.disaster.type
                             : (ZDisasterType)a_random_int(Z_DISASTER_NUM);

        z_game_log(z_fonts.cyan, text[clue]);
    }
}

static void passDecade(void)
{
    z_data_inc(z_game.data, Z_DATA_YEAR, 10);

    if(z_data_getReal(z_game.data, Z_DATA_YEAR) % 100 == 0) {
        z_game_log(z_fonts.lightGray, "A century passed");
        z_data_tick(z_game.data, false);

        a_state_push("investment");
    } else {
        z_game_log(z_fonts.lightGray, "10 years passed");

        if(!adjustPopulation()) {
            disasterClue();
        }
    }
}

static void drawScene(void)
{
    a_pixel_setPixel(z_colors.darkGray);
    a_draw_fill();

    ASprite* clouds = z_graphics_getFrame(z_graphics_get("clouds"), 0);
    a_sprite_blit(clouds, 0, z_game.tilesBaseline - a_sprite_height(clouds));

    A_LIST_ITERATE(z_game.structures, ZStructure*, s) {
        ASprite* sprite = z_graphics_getNextFrame(s->graphic);
        a_sprite_blit(sprite,
                      s->tile * Z_TILE_WIDTH,
                      z_game.tiles[s->tile].y
                        - a_sprite_height(sprite));
    }

    for(unsigned i = z_game.tilesNum; i--; ) {
        int x = (int)i * Z_TILE_WIDTH;

        a_pixel_setPixel(z_colors.lightGray);
        a_draw_hline(x, x + Z_TILE_WIDTH, z_game.tiles[i].y);
        a_pixel_setPixel(z_colors.darkGray);
        a_draw_rectangle(x,
                         z_game.tiles[i].y + 1,
                         Z_TILE_WIDTH,
                         a_screen_height() - z_game.tiles[i].y - 1);
    }
}

static void drawIcons(void)
{
    ASprite* sprite;
    const ZGraphic* icons = z_graphics_get("iconsLight");

    a_font_setFace(z_fonts.lightGray);

    int x = 247;
    int y = 2;
    int barW = 48;
    int barH = 8;
    int barSpacing = 4;

    a_font_push();

    for(ZDataType d = 0; d < Z_DATA_NUM; d++) {
        sprite = z_graphics_getFrame(icons, d);
        a_sprite_blit(sprite, x, y);
        x += a_sprite_width(sprite) + 1;

        if(d == Z_DATA_YEAR) {
            a_font_setCoords(x, y + 1);
            a_font_setFace(z_fonts.lightGray);
            a_font_setAlign(A_FONT_ALIGN_LEFT);
            a_font_text("YEAR");
            a_font_setFace(z_fonts.cyan);
            a_font_setAlign(A_FONT_ALIGN_MONOSPACED);
            a_font_textf("%d", z_data_getShow(z_game.data, Z_DATA_YEAR));
            x = 246;
            y = 12;
        } else if(d == Z_DATA_POPULATION) {
            a_font_setCoords(x, y + 1);
            a_font_setFace(z_fonts.lightGray);
            a_font_setAlign(A_FONT_ALIGN_LEFT);
            a_font_text("POP");
            a_font_setFace(z_fonts.cyan);
            a_font_setAlign(A_FONT_ALIGN_MONOSPACED);
            a_font_textf("%d",
                         a_fix_fixtoi(z_data_getShow(z_game.data,
                                                     Z_DATA_POPULATION)));
            x = 2;
            y = 7;
        } else {
            z_graphics_drawBar(z_colors.cyan,
                               z_colors.lightGray,
                               z_data_getShow(z_game.data, d),
                               256,
                               x,
                               y,
                               barW,
                               barH);
            x += barW + barSpacing;
        }
    }

    a_font_pop();
}

void z_game_log(unsigned Font, const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    z_log_log(z_game.log, Font, Format, args);

    va_end(args);
}

bool z_game_kill(AFix Number)
{
    z_data_inc(z_game.data, Z_DATA_POPULATION, -Number);

    // TODO: destroy buildings

    if(a_fix_fixtoi(z_data_getReal(z_game.data, Z_DATA_POPULATION)) == 0) {
        z_game_log(z_fonts.lightGray, "");
        z_game_log(z_fonts.lightGray, "Everybody died");
        z_game_log(z_fonts.lightGray, "2 seconds of silence...");

        a_state_push("over");

        return true;
    }

    return false;
}

bool z_game_tick(void)
{
    return z_log_tick(z_game.log) && z_data_tick(z_game.data, true);
}

void z_game_draw(ZHelpScreen HelpScreen)
{
    a_pixel_push();

    drawScene();
    drawIcons();
    z_log_draw(z_game.log, 2, 130);
    z_graphics_drawHelp(HelpScreen);

    a_pixel_pop();
}

A_STATE(play)
{
    A_STATE_INIT
    {
        initGame();
    }

    A_STATE_BODY
    {
        A_STATE_LOOP
        {
            if(a_button_getOnce(z_controls.primary)) {
                passDecade();
            }

            z_game_tick();

            A_STATE_LOOP_DRAW
            {
                z_game_draw(Z_HELP_MAINGAME);
            }
        }
    }

    A_STATE_FREE
    {
        freeGame();
    }
}

A_STATE(over)
{
    A_STATE_BODY
    {
        ATimer* t = a_timer_new(A_TIMER_MS, 2000);
        a_timer_start(t);

        A_STATE_LOOP
        {
            if(a_timer_isExpired(t)) {
                a_timer_stop(t);

                z_game_log(z_fonts.lightGray, "");
                z_game_log(z_fonts.magenta, "GAME OVER");
            }

            if(!a_timer_isRunning(t) && a_button_getOnce(z_controls.start)) {
                a_state_push("hiscore");
            }

            z_game_tick();

            A_STATE_LOOP_DRAW
            {
                z_game_draw(Z_HELP_GAMEOVER);
            }
        }

        a_timer_free(t);
    }
}

A_STATE(hiscore)
{
    A_STATE_BODY
    {
        int years = z_data_getReal(z_game.data, Z_DATA_YEAR);
        int evolutions = z_game.evolutions;
        int population = a_fix_fixtoi(z_data_getReal(z_game.data, Z_DATA_POPULATION));

        uint32_t score = (uint32_t)((years / 10) * (1 + evolutions) + population);

        z_game_log(z_fonts.lightGray, "");
        z_game_log(z_fonts.lightGray,
                   "Evolved %d times over %d years",
                   z_game.evolutions,
                   years);

        z_game_log(z_fonts.cyan, "FINAL SCORE %u", score);

        if(score > z_game.hiscore) {
            z_game_log(z_fonts.magenta,
                       "New hiscore, old was %u",
                       z_game.hiscore);

            AFile* f = a_file_open(Z_HISCORE_FILE, "wb");

            if(f) {
                a_file_writePrefix(f, Z_HISCORE_KEY);
                a_file_write(f, &score, sizeof(score));
                a_file_close(f);
            }

            z_game.hiscore = score;
        } else {
            z_game_log(z_fonts.lightGray, "You did not get a hiscore");
        }

        A_STATE_LOOP
        {
            if(z_game_tick() && a_button_getOnce(z_controls.start)) {
                a_state_popUntil("title");
            }

            A_STATE_LOOP_DRAW
            {
                z_game_draw(Z_HELP_GAMEOVER);
            }
        }
    }
}

A_STATE(evolution)
{
    A_STATE_BODY
    {
        z_game.evolutions++;

        z_game_log(z_fonts.cyan, "");
        z_game_log(z_fonts.cyan, "Congratulations!");
        z_game_log(z_fonts.cyan, "Your world EVOLVED");

        A_STATE_LOOP
        {
            if(z_game_tick()) {
                z_game_log(z_fonts.cyan, "");
                a_state_popUntil("play");
            }

            A_STATE_LOOP_DRAW
            {
                z_game_draw(Z_HELP_NONE);
            }
        }
    }

    A_STATE_FREE
    {
        // TODO: change buildings
        z_data_set(z_game.data, Z_DATA_SCIENCE, 0);
    }
}
