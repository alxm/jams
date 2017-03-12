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

#include "util_graphics.h"

#include "component_cargo.h"
#include "component_interact.h"
#include "component_sprite.h"
#include "component_trade.h"

#include "state_game.h"

static void transaction(ZCompTrade* Trade, AEntity* Buyer, AEntity* Seller, ZCargoType Product, bool PlayerBuys)
{
    ZCompCargo* bCargo = a_entity_requireComponent(Buyer, "cargo");
    ZCompCargo* sCargo = a_entity_requireComponent(Seller, "cargo");

    ZCompInteract* pInter = a_entity_requireComponent(Buyer, "interact");
    ZCompInteract* mInter = a_entity_requireComponent(Seller, "interact");

    const char* bName = z_comp_interact_getName(pInter);
    const char* sName = z_comp_interact_getName(mInter);

    int price = z_comp_trade_getPrice(Trade, Product, PlayerBuys);

    if(z_comp_cargo_getNum(sCargo, Product) < 1) {
        z_game_log("%s has no %s in stock",
                   sName,
                   z_comp_cargo_getName(Product, true));
    } else if(price > z_comp_cargo_getNum(bCargo, Z_CARGO_TYPE_CREDS)) {
        z_game_log("%s does not have enough creds", bName);
    } else {
        z_comp_cargo_take(sCargo, bCargo, Z_CARGO_TYPE_CREDS, price);
        z_comp_cargo_take(bCargo, sCargo, Product, 1);

        z_game_log("  [%s transaction complete]",
                   PlayerBuys ? "buy" : "sell");
    }
}

void z_system_tradeTick(AEntity* Merchant)
{
    ZCompTrade* trade = a_entity_requireComponent(Merchant, "trade");

    if(!z_comp_trade_getOn(trade)) {
        return;
    }

    AMenu* menu = z_comp_trade_getMenu(trade);

    a_menu_input(menu);

    if(a_menu_finished(menu)) {
        if(a_menu_cancel(menu)
            || a_menu_choice(menu) == a_menu_numItems(menu) - 1) {

            a_menu_reset(menu);
            z_game_tradeOff(Merchant);
        } else {
            unsigned choice = a_menu_choice(menu);

            switch(choice) {
                case Z_TRADE_MENU_BUY_FUEL:
                case Z_TRADE_MENU_BUY_MINERALS: {
                    transaction(trade,
                                z_game_getPlayer(),
                                Merchant,
                                Z_CARGO_TYPE_FUEL + choice,
                                true);
                } break;

                case Z_TRADE_MENU_SELL_FUEL:
                case Z_TRADE_MENU_SELL_MINERALS: {
                    transaction(trade,
                                Merchant,
                                z_game_getPlayer(),
                                Z_CARGO_TYPE_FUEL
                                    + choice - Z_TRADE_MENU_SELL_FUEL,
                                false);
                } break;

                case Z_TRADE_MENU_GET_REPAIRS: {
                    z_game_log("  [service unavailable]");
                } break;
            }

            a_menu_keepRunning(menu);
        }
    }
}

static void drawHeader(AEntity* Entity, bool AlignLeft)
{
    ZCompInteract* interact = a_entity_requireComponent(Entity, "interact");
    ZCompSprite* sprite = a_entity_requireComponent(Entity, "sprite");

    const char* name = z_comp_interact_getName(interact);
    ASprite* graphic = z_comp_sprite_getFrameFacing(
                sprite,
                AlignLeft ? Z_SPRITE_DIRECTION_RIGHT : Z_SPRITE_DIRECTION_LEFT);
    int screenW = a_screen_width();

    a_font_push();
    a_font_setFace(A_FONT_FACE_WHITE);
    int fontH = a_font_getLineHeight();

    a_pixel_setHex(0x432949);

    if(AlignLeft) {
        a_sprite_blit(graphic, 8, 8);
        a_font_setCoords(8 + Z_MAP_TILE_DIM + 4,
                         10 + (Z_MAP_TILE_DIM - fontH) / 2);
        a_font_text(name);
        a_draw_hline(4, screenW / 2 - Z_MAP_TILE_DIM, 8 + Z_MAP_TILE_DIM + 4);
    } else {
        a_sprite_blit(graphic, screenW - 8 - Z_MAP_TILE_DIM, 8);
        a_font_setCoords(screenW - 8 - Z_MAP_TILE_DIM - 4,
                         10 + (Z_MAP_TILE_DIM - fontH) / 2);
        a_font_setAlign(A_FONT_ALIGN_RIGHT);
        a_font_text(name);
        a_draw_hline(screenW / 2 + Z_MAP_TILE_DIM,
                     screenW - 4,
                     8 + Z_MAP_TILE_DIM + 4);
    }

    a_font_pop();
}

void z_system_tradeDraw(AEntity* Merchant)
{
    ZCompTrade* trade = a_entity_requireComponent(Merchant, "trade");
    ZCompCargo* cargo = a_entity_requireComponent(Merchant, "cargo");

    if(!z_comp_trade_getOn(trade)) {
        return;
    }

    a_pixel_push();
    a_pixel_setHex(0x151225);
    a_pixel_setBlend(A_PIXEL_BLEND_RGBA);
    a_pixel_setAlpha(9 * A_PIXEL_ALPHA_MAX / 10);
    a_draw_rectangle(0, 0, a_screen_width(), Z_MAP_PIXEL_H);
    a_pixel_pop();

    drawHeader(z_game_getPlayer(), true);
    drawHeader(Merchant, false);

    AMenu* menu = z_comp_trade_getMenu(trade);
    int x = 4;
    int y = 8 + Z_MAP_TILE_DIM + 8;
    int screenW = a_screen_width();
    int menuItemW = screenW - 8;
    int menuItemH = 4 + a_font_getLineHeight() + 4;

    A_MENU_ITERATE(menu, char*, title) {
        if(A_LIST_IS_LAST()) {
            y = Z_MAP_PIXEL_H - menuItemH - 4;
        }

        a_pixel_push();
        a_pixel_setHex(0x432949);

        if(a_menu_isSelected(menu, title)) {
            a_pixel_setBlend(A_PIXEL_BLEND_RGB50);
            a_font_setFace(z_fonts.lightOrange);
        } else {
            a_pixel_setBlend(A_PIXEL_BLEND_RGB25);
            a_font_setFace(A_FONT_FACE_WHITE);
        }

        a_draw_rectangle(x, y, menuItemW, menuItemH);
        a_pixel_pop();
        a_font_setCoords(x + 4, y + 4);
        a_font_text(title);

        a_font_push();
        a_font_setCoords(screenW - 8, y + 4);
        a_font_setAlign(A_FONT_ALIGN_RIGHT);
        a_font_setFace(z_fonts.lightBlue);

        switch(A_LIST_INDEX()) {
            case Z_TRADE_MENU_BUY_FUEL:
            case Z_TRADE_MENU_BUY_MINERALS: {
                ZCargoType type = Z_CARGO_TYPE_FUEL + A_LIST_INDEX();
                int num = z_comp_cargo_getNum(cargo, type);
                int price = z_comp_trade_getPrice(trade, type, true);
                a_font_textf("Has %d for %d creds each", num, price);
            } break;

            case Z_TRADE_MENU_SELL_FUEL:
            case Z_TRADE_MENU_SELL_MINERALS: {
                ZCargoType type = Z_CARGO_TYPE_FUEL + A_LIST_INDEX() - 2;
                int price = z_comp_trade_getPrice(trade, type, false);
                a_font_textf("Will buy for %d creds each", price);
            } break;

            case Z_TRADE_MENU_GET_REPAIRS: {
                a_font_textf("Not available");
            } break;
        }

        a_font_pop();
        y += menuItemH + 2;
    }
}
