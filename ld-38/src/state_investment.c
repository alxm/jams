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

#include "util_data.h"
#include "util_graphics.h"
#include "util_log.h"
#include "util_period.h"

#include "state_game.h"

static bool handleMenu(void)
{
    a_menu_handleInput(z_game.menu);
    AMenuState menuState = a_menu_getState(z_game.menu);

    if(menuState == A_MENU_STATE_SELECTED) {
        unsigned choice = a_menu_getSelectedIndex(z_game.menu);
        ZDataType data = Z_DATA_DEFENSE + choice;
        const char* dataName = a_menu_getSelectedItem(z_game.menu);

        if(z_data_isMax(z_game.data, data)) {
            z_game_log(z_fonts.lightGray,
                       "  %s is maxed out, try something else",
                       dataName);
            a_menu_keepRunning(z_game.menu);
            return false;
        }

        int currentValue = z_data_getReal(z_game.data, data);
        z_data_inc(z_game.data, data, 64);
        int newValue = z_data_getReal(z_game.data, data);

        int century = 1 + z_data_getReal(z_game.data, Z_DATA_YEAR) / 100;
        const char* suffix = "th";

        switch(century % 10) {
            case 1:
                suffix = "st";
                break;

            case 2:
                suffix = "nd";
                break;

            case 3:
                suffix = "rd";
                break;
        }

        z_game_log(z_fonts.lightGray,
                   "Entering the %d%s Century, the CENTURY OF %s",
                   century,
                   suffix,
                   dataName);

        z_game_log(z_fonts.lightGray,
                   "  Gained %d %s points",
                   newValue - currentValue,
                   dataName);

        if(z_data_isMax(z_game.data, Z_DATA_SCIENCE)) {
            a_state_push("evolution");
        }

        return true;
    }

    return false;
}

static void drawMenu(void)
{
    const ZGraphic* icons = z_graphics_get("iconsDark");

    a_font_setFace(z_fonts.darkGray);

    int startX = 2;
    int y = 22;
    int w = a_screen_width() - 4;
    int h = a_font_getLineHeight() + 2;

    a_pixel_setPixel(z_colors.magenta);
    a_draw_rectangle(startX, y, w, h);
    a_font_setCoords(startX + 1, y + 2);
    a_font_text("Make an INVESTMENT for the next CENTURY - choose wisely");
    y += h + 2;

    A_LIST_ITERATE(a_menu_getItems(z_game.menu), char*, title) {
        int x = startX;
        ASprite* sprite = z_graphics_getFrame(icons,
                                              Z_DATA_DEFENSE + A_LIST_INDEX());

        a_font_setFace(z_fonts.darkGray);
        a_pixel_setPixel(z_colors.lightGray);

        a_draw_rectangle(x, y, w, h);

        a_sprite_blit(sprite, x + 1, y + 1);
        x += 9 + 2;

        a_font_setCoords(x + 1, y + 2);
        if(a_menu_isItemSelected(z_game.menu, title)) {
            a_font_setFace(z_fonts.cyan);
        } else {
            a_font_setFace(z_fonts.mediumGray);
        }
        a_font_text("Invest in ");
        a_font_text(title);

        y += h + 1;
    }
}

A_STATE(investment)
{
    static AScreen* captured = NULL;

    A_STATE_INIT
    {
        a_fade_screens(a_fps_msToFrames(500));
        captured = a_screen_dup();
        z_game_draw(Z_HELP_INVESTMENT);
        drawMenu();
    }

    A_STATE_BODY
    {
        bool selectedInvestment = false;

        A_STATE_LOOP
        {
            bool gameUpToDate = z_game_tick();

            if(!selectedInvestment) {
                selectedInvestment = handleMenu();
            } else if(gameUpToDate) {
                a_state_replace("disaster");
            }

            A_STATE_LOOP_DRAW
            {
                z_game_draw(Z_HELP_INVESTMENT);
                drawMenu();
            }
        }
    }

    A_STATE_FREE
    {
        a_fade_screens(a_fps_msToFrames(250));
        z_game_draw(Z_HELP_MAINGAME);

        a_screen_free(captured);
        a_menu_reset(z_game.menu);
    }
}
