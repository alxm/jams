/*
    Copyright 2016 Alex Margarit

    This file is part of Rushed Dinosaur Dare.

    Rushed Dinosaur Dare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Rushed Dinosaur Dare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rushed Dinosaur Dare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "game.h"

Game g_game;

static void game_checkState(void)
{
    if(buggy_getX(g_game.buggy) < land_getBufferOffset(g_game.ground)) {
        a_button_unpress(g_data.jump);
        a_button_unpress(g_data.shoot);
        a_state_replace("gameover");
    }
}

A_STATE(newgame)
{
    A_STATE_BODY
    {
        land_init();

        g_game.hills = land_new(LAND_TYPE_HILLS, 128, A_FIX_ONE / 2);
        g_game.ground = land_new(LAND_TYPE_GROUND, 160, A_FIX_ONE / 1);
        g_game.bones = bones_new(g_game.ground);
        g_game.buggy = buggy_new(g_game.ground);
        g_game.dinos = a_list_new();

        a_state_push("rungame");
    }

    A_STATE_FREE
    {
        land_free(g_game.hills);
        land_free(g_game.ground);
        bones_free(g_game.bones);
        buggy_free(g_game.buggy);

        A_LIST_ITERATE(g_game.dinos, DinoInstance*, d) {
            dino_free(d);
        }

        a_list_free(g_game.dinos);
    }
}

A_STATE(rungame)
{
    A_STATE_BODY
    {
        A_STATE_RUN
        {
            A_STATE_LOOP
            {
                if(a_button_get(g_data.left)) {
                    buggy_moveLeft(g_game.buggy);
                }

                if(a_button_get(g_data.right)) {
                    buggy_moveRight(g_game.buggy);
                }

                if(a_button_getAndUnpress(g_data.jump)) {
                    buggy_jump(g_game.buggy);
                }

                if(a_button_getAndUnpress(g_data.shoot)) {
                    buggy_shoot(g_game.buggy);
                }

                land_tick(g_game.hills);
                land_tick(g_game.ground);
                buggy_tick(g_game.buggy);
                bones_tick(g_game.bones);
                dinos_tick(g_game.dinos);

                game_checkState();

                a_pixel_setPixel(0);
                a_draw_fill();

                land_draw(g_game.hills);
                buggy_draw(g_game.buggy);
                bones_draw(g_game.bones);
                land_draw(g_game.ground);
                dinos_draw(g_game.dinos);
            }
        }

        A_STATE_PAUSE
        {
            A_STATE_LOOP
            {
                // TODO
            }
        }
    }
}

A_STATE(gameover)
{
    A_STATE_BODY
    {
        A_STATE_LOOP
        {
            a_pixel_setPixel(0);
            a_draw_fill();

            a_font_setFace(g_data.fontMagenta);
            a_font_setAlign(A_FONT_ALIGN_MIDDLE);
            a_font_setCoords(a_screen_width() / 2, a_screen_height() / 2 - 30);
            a_font_text("The Stegosaurus pushed you back to 1981,");
            a_font_setCoords(a_screen_width() / 2, a_screen_height() / 2 - 20);
            a_font_text("they have multi-color screens there!");

            a_font_setFace(A_FONT_FACE_WHITE);
            a_font_setCoords(a_screen_width() / 2, a_screen_height() / 2);
            a_font_text("Thanks for playing! I will build this into a full game,");
            a_font_setCoords(a_screen_width() / 2, a_screen_height() / 2 + 10);
            a_font_text("check www.alxm.org for updates");

            a_font_setFace(g_data.fontCyan);
            a_font_setCoords(a_screen_width() / 2, a_screen_height() / 2 + 30);
            a_font_text("Press ESC to close");
        }
    }
}
