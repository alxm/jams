/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of Coffin Digital, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "obj_market.h"

#include "obj_game.h"
#include "util_color.h"
#include "util_font.h"

#define Z_TICKER_X 75
#define Z_TICKER_Y 164
#define Z_TICKER_W 124
#define Z_TICKER_H 7

#define Z_TICKER_MSG "//// TRI %d // SQR %d // CRC %d ////"

typedef struct {
    int values[Z_RESOURCE_NUM];
    int offset;
} NMarket;

static NMarket g_market;

static const int g_rangeStart[Z_RESOURCE_NUM][2] = {
    [Z_RESOURCE_TRIANGLE] = {4, 12},
    [Z_RESOURCE_SQUARE] = {6, 10},
    [Z_RESOURCE_CIRCLE] = {8, 20},
};

void n_market_new(void)
{
    memset(&g_market, 0, sizeof(NMarket));

    for(int i = Z_RESOURCE_NUM; i--; ) {
        g_market.values[i] = a_random_range(
                                g_rangeStart[i][0], g_rangeStart[i][1] + 1);
    }

    g_market.offset = Z_TICKER_X + Z_TICKER_W;
}

void n_market_tick(void)
{
    g_market.offset--;

    a_font_fontSet(u_font_get(U_FONT_DEFAULT));

    int w = a_font_widthGetf(Z_TICKER_MSG,
                             g_market.values[Z_RESOURCE_TRIANGLE],
                             g_market.values[Z_RESOURCE_SQUARE],
                             g_market.values[Z_RESOURCE_CIRCLE]);

    if(g_market.offset + w < Z_TICKER_X) {
        g_market.offset = Z_TICKER_X + Z_TICKER_W;
    }
}

void n_market_draw(void)
{
    a_screen_clipSet(Z_TICKER_X, Z_TICKER_Y, Z_TICKER_W, Z_TICKER_H);

    a_font_coordsSet(g_market.offset, Z_TICKER_Y);
    a_font_fontSet(u_font_get(U_FONT_DEFAULT));
    a_font_alignSet(A_FONT_ALIGN_LEFT);

    a_color_baseSetPixel(u_color_get(U_COLOR_GRAY_LIGHT));
    a_color_fillBlitSet(true);

    a_font_printf(Z_TICKER_MSG,
                  g_market.values[Z_RESOURCE_TRIANGLE],
                  g_market.values[Z_RESOURCE_SQUARE],
                  g_market.values[Z_RESOURCE_CIRCLE]);

    a_color_fillBlitSet(false);

    a_screen_clipReset();
}

int n_market_valueGet(ZResourceId Id)
{
    return g_market.values[Id];
}
