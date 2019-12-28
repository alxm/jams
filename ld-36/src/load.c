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

#include "load.h"

GlobalData g_data;

A_STATE(load)
{
    A_STATE_BODY
    {
        ASprite* sheet = a_sprite_fromFile("gfx/tiles.png");

        g_data.boneGraphics = a_spriteframes_new(sheet, 0, 0, 0);
        g_data.grassGraphics = a_spriteframes_new(sheet, 0, 18, 0);
        g_data.buggyBackTracks = a_spriteframes_new(sheet, 0, 32, 6);
        g_data.buggyFrontTracks = a_spriteframes_new(sheet, 0, 40, 6);
        g_data.buggyChassis = a_sprite_new(sheet, 0, 48);
        g_data.buggyCabin = a_sprite_new(sheet, 0, 56);
        g_data.stegoBody = a_sprite_new(sheet, 0, 88);
        g_data.stegoLegs = a_spriteframes_new(sheet, 0, 121, 12);

        a_sprite_free(sheet);

        g_data.up = a_input_new("pc.Up");
        g_data.down = a_input_new("pc.Down");
        g_data.left = a_input_new("pc.Left");
        g_data.right = a_input_new("pc.Right");
        g_data.jump = a_input_new("pc.z");
        g_data.shoot = a_input_new("pc.x");

        g_data.fontCyan = a_font_copy(A_FONT_FACE_WHITE, a_pixel_make(0x55, 0xff, 0xff));
        g_data.fontMagenta = a_font_copy(A_FONT_FACE_WHITE, a_pixel_make(0xff, 0x55, 0xff));

        a_button_unpress(g_data.jump);

        A_STATE_LOOP
        {
            int w;
            a_pixel_setPixel(0);
            a_draw_fill();

            a_font_setFace(g_data.fontCyan);
            a_font_setAlign(A_FONT_ALIGN_MIDDLE);
            a_font_setCoords(a_screen_width() / 2, a_screen_height() / 2 - 40);
            a_font_text("A game for the Ludum Dare 36 48h Compo");
            a_font_setCoords(a_screen_width() / 2, a_screen_height() / 2 - 30);
            a_font_text("www.alxm.org - August 28, 2016");

            a_font_setAlign(A_FONT_ALIGN_LEFT);
            w = a_font_widthf("Press %s to jump", a_input_name(g_data.jump));
            a_font_setCoords(a_screen_width() / 2 - w / 2, a_screen_height() / 2 - 10);
            a_font_setFace(A_FONT_FACE_WHITE);
            a_font_text("Press ");
            a_font_setFace(g_data.fontMagenta);
            a_font_text(a_input_name(g_data.jump));
            a_font_setFace(A_FONT_FACE_WHITE);
            a_font_text(" to jump");

            a_font_setAlign(A_FONT_ALIGN_LEFT);
            w = a_font_widthf("Press %s and %s to move", a_input_name(g_data.left), a_input_name(g_data.right));
            a_font_setCoords(a_screen_width() / 2 - w / 2, a_screen_height() / 2);
            a_font_setFace(A_FONT_FACE_WHITE);
            a_font_text("Press ");
            a_font_setFace(g_data.fontMagenta);
            a_font_text(a_input_name(g_data.left));
            a_font_setFace(A_FONT_FACE_WHITE);
            a_font_text(" and ");
            a_font_setFace(g_data.fontMagenta);
            a_font_text(a_input_name(g_data.right));
            a_font_setFace(A_FONT_FACE_WHITE);
            a_font_text(" to move");

            a_font_setAlign(A_FONT_ALIGN_LEFT);
            w = a_font_widthf("Press the %s key to start", a_input_name(g_data.jump));
            a_font_setCoords(a_screen_width() / 2 - w / 2, a_screen_height() / 2 + 20);
            a_font_setFace(A_FONT_FACE_WHITE);
            a_font_text("Press the ");
            a_font_setFace(g_data.fontMagenta);
            a_font_text(a_input_name(g_data.jump));
            a_font_setFace(A_FONT_FACE_WHITE);
            a_font_text(" key to start");



            if(a_button_getAndUnpress(g_data.jump)) {
                a_state_push("newgame");
            }
        }
    }

    A_STATE_FREE
    {
        a_spriteframes_free(g_data.boneGraphics, false);
        a_spriteframes_free(g_data.grassGraphics, false);
    }
}
