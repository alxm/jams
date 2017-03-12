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

typedef struct ZCompTrade ZCompTrade;

typedef enum {
    Z_TRADE_MENU_BUY_FUEL,
    Z_TRADE_MENU_BUY_MINERALS,
    Z_TRADE_MENU_SELL_FUEL,
    Z_TRADE_MENU_SELL_MINERALS,
    Z_TRADE_MENU_GET_REPAIRS,
    Z_TRADE_MENU_LEAVE,
    Z_TRADE_MENU_NUM
} ZTradeMenuItem;

extern size_t z_comp_trade_size(void);
extern void z_comp_trade_init(ZCompTrade* Trade);
AComponentFree z_comp_trade_free;

extern bool z_comp_trade_getOn(const ZCompTrade* Trade);
extern void z_comp_trade_setOn(ZCompTrade* Trade, bool On);

extern AMenu* z_comp_trade_getMenu(const ZCompTrade* Trade);
extern int z_comp_trade_getPrice(const ZCompTrade* Trade, ZCargoType Product, bool PlayerBuys);
