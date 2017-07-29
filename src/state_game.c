/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Despot 3900 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Despot 3900 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Despot 3900.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#define Z_YEARS_TO_MONTHS(Years)  ((Years) * 12)
#define Z_MONTHS_TO_YEARS(Months) ((Months) / 12)

typedef struct ZDespot {
    int dobInMonths;
    int health;
    int wealth;
    int popularity;
    int loyalty;
} ZDespot;

typedef struct ZGame {
    int timeInMonths;
    ZDespot despot;
} ZGame;

static void game_init(ZGame* Game)
{
    Game->timeInMonths = Z_YEARS_TO_MONTHS(3900);
    Game->despot.dobInMonths = Game->timeInMonths - Z_YEARS_TO_MONTHS(30);
    Game->despot.health = 80;
    Game->despot.wealth = 1000;
    Game->despot.popularity = 50;
    Game->despot.loyalty = 50;
}

static void game_free(ZGame* Game)
{
    A_UNUSED(Game);
}

A_STATE(game)
{
    static ZGame game;

    A_STATE_INIT
    {
        game_init(&game);
    }

    A_STATE_BODY
    {
        int x = a_screen_getWidth() / 2;
        int y = a_screen_getHeight() / 2;
        int dim = a_screen_getWidth() / 4;

        AInputButton* up = a_button_new("key.up");
        AInputButton* down = a_button_new("key.down");
        AInputButton* left = a_button_new("key.left");
        AInputButton* right = a_button_new("key.right");

        A_STATE_LOOP
        {
            if(a_button_getPressed(up)) {
                y--;
            } else if(a_button_getPressed(down)) {
                y++;
            }

            if(a_button_getPressed(left)) {
                x--;
            } else if(a_button_getPressed(right)) {
                x++;
            }

            A_STATE_LOOP_DRAW
            {
                a_pixel_setHex(0xaaff88);
                a_draw_fill();

                a_pixel_setHex(0xffaa44);
                a_draw_rectangle(x - dim / 2, y - dim / 2, dim, dim);
            }
        }
    }

    A_STATE_FREE
    {
        game_free(&game);
    }
}
