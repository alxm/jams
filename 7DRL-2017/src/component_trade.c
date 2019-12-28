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

#include <a2x.h>

#include "util_controls.h"

#include "component_cargo.h"
#include "component_trade.h"

struct ZCompTrade {
    bool on;
    AMenu* menu;
    int buyPrices[Z_CARGO_TYPE_NUM]; // Merchant buys for this much creds
    int sellPrices[Z_CARGO_TYPE_NUM]; // Merchant sells for this much creds
    bool doesRepairs;
    int repairPrice;
    int repairsLeft;
};

size_t z_comp_trade_size(void)
{
    return sizeof(ZCompTrade);
}

void z_comp_trade_init(ZCompTrade* Trade)
{
    Trade->menu = a_menu_new(z_controls.down,
                             z_controls.up,
                             z_controls.primary,
                             z_controls.secondary);

    char* menuItems[Z_TRADE_MENU_NUM];
    menuItems[Z_TRADE_MENU_BUY_FUEL] = "Buy Fuel";
    menuItems[Z_TRADE_MENU_BUY_MINERALS] = "Buy Minerals";
    menuItems[Z_TRADE_MENU_SELL_FUEL] = "Sell Fuel";
    menuItems[Z_TRADE_MENU_SELL_MINERALS] = "Sell Minerals";
    menuItems[Z_TRADE_MENU_GET_REPAIRS] = "Get Repairs";
    menuItems[Z_TRADE_MENU_LEAVE] = "Leave";

    for(int i = 0; i < Z_TRADE_MENU_NUM; i++) {
        a_menu_addItem(Trade->menu, menuItems[i]);
    }

    Trade->buyPrices[Z_CARGO_TYPE_CREDS] = 1;
    Trade->sellPrices[Z_CARGO_TYPE_CREDS] = 1;

    Trade->buyPrices[Z_CARGO_TYPE_FUEL] = 2 + a_random_getInt(5);
    Trade->sellPrices[Z_CARGO_TYPE_FUEL] = 4 + a_random_getInt(5);

    Trade->buyPrices[Z_CARGO_TYPE_MINERALS] = 4 + a_random_getInt(9);
    Trade->sellPrices[Z_CARGO_TYPE_MINERALS] = 8 + a_random_getInt(9);

    Trade->doesRepairs = true;
    Trade->repairPrice = 1;
    Trade->repairsLeft = 1 + a_random_getInt(5);
}

void z_comp_trade_free(void* Self)
{
    ZCompTrade* trade = Self;

    a_menu_free(trade->menu);
}

bool z_comp_trade_getOn(const ZCompTrade* Trade)
{
    return Trade->on;
}

void z_comp_trade_setOn(ZCompTrade* Trade, bool On)
{
    Trade->on = On;
}

AMenu* z_comp_trade_getMenu(const ZCompTrade* Trade)
{
    return Trade->menu;
}

int z_comp_trade_getPrice(const ZCompTrade* Trade, ZCargoType Product, bool PlayerBuys)
{
    if(PlayerBuys) {
        return Trade->sellPrices[Product];
    } else {
        return Trade->buyPrices[Product];
    }
}

bool z_comp_trade_getDoesRepairs(const ZCompTrade* Trade)
{
    return Trade->doesRepairs;
}

void z_comp_trade_setDoesRepairs(ZCompTrade* Trade, bool DoesRepairs)
{
    Trade->doesRepairs = DoesRepairs;
}

int z_comp_trade_getRepairPrice(const ZCompTrade* Trade)
{
    return Trade->repairPrice;
}

void z_comp_trade_doRepair(ZCompTrade* Trade)
{
    if(!Trade->repairsLeft--) {
        Trade->doesRepairs = false;
    }
}
