/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Pestering Peddler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Pestering Peddler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pestering Peddler.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#include "state_game.h"

#include "util_bar.h"
#include "util_colors.h"
#include "util_fonts.h"
#include "util_sprites.h"

#include "component_alarm.h"

void z_system_alarmTick(AEntity* Entity)
{
    ZCompAlarm* alarm = a_entity_requireComponent(Entity, "alarm");

    if(!z_comp_alarm_isOn(alarm)) {
        return;
    }

    AFix value = z_comp_alarm_getValue(alarm) + A_FIX_ONE / 8;

    z_comp_alarm_setValue(alarm, value);

    if(a_fix_fixtoi(value) >= 100) {
        z_state_game_gameOver(a_entity_getContext(Entity));
    }
}

void z_system_alarmDraw(AEntity* Entity)
{
    ZCompAlarm* alarm = a_entity_requireComponent(Entity, "alarm");

    int x = 8;
    int y = a_screen_getHeight() - 20;
    int width = a_screen_getWidth() - 2 * x;
    int height = 11;
    int value = a_fix_fixtoi(z_comp_alarm_getValue(alarm));

    int part1Width = a_math_min(width * value / 100, width);

    z_util_bar_draw(z_util_colors.red2,
                    z_util_colors.gray4,
                    value,
                    100,
                    x,
                    y,
                    width,
                    height);

    ASprite* poop1 = z_util_sprites_getSingle("poop1");
    ASprite* poop2 = z_util_sprites_getSingle("poop2");

    a_screen_clipSet(x, y, part1Width, height);
    a_sprite_blit(poop1, x + 2, y + 2);
    a_font_setCoords(x + a_sprite_getWidth(poop1) + 4, y + 2);
    a_font_setFont(z_util_fonts.blue2);
    a_font_print("SCAM-O-METER");

    a_screen_clipSet(x + part1Width, y, width - part1Width, height);
    a_sprite_blit(poop2, x + 2, y + 2);
    a_font_setCoords(x + a_sprite_getWidth(poop2) + 4, y + 2);
    a_font_setFont(z_util_fonts.gray2);
    a_font_print("SCAM-O-METER");

    a_screen_clipReset();
}
