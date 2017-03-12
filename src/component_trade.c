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

#include "component_trade.h"

struct ZCompTrade {
    bool on;
    AMenu* menu;
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

    a_menu_addItem(Trade->menu, "Buy Fuel");
    a_menu_addItem(Trade->menu, "Buy Minerals");
    a_menu_addItem(Trade->menu, "Sell Fuel");
    a_menu_addItem(Trade->menu, "Sell Minerals");
    a_menu_addItem(Trade->menu, "Get Repairs");
    a_menu_addItem(Trade->menu, "Leave");
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
