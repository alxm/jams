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

#include "util_colors.h"
#include "util_controls.h"
#include "util_fonts.h"
#include "util_sprites.h"
#include "util_tiles.h"

#include "component_alarm.h"
#include "component_map.h"
#include "component_position.h"

#include "entity_alarm.h"
#include "entity_item.h"
#include "entity_log.h"
#include "entity_map.h"
#include "entity_pigeon.h"
#include "entity_player.h"

struct ZStateGame {
    AEntity* log;
    AEntity* map;
    AEntity* player;
    AEntity* alarm;
    AColMap* volumeColMap;
};

static bool g_pressedBuy;
static bool g_returned;

static int g_scamCount;

A_STATE(game)
{
    static ZStateGame game;

    A_STATE_INIT
    {
        a_system_tick("ttl input ai move animate logTick alarmTick");
        a_system_draw("mapDraw spriteDraw logDraw alarmDraw");

        game.log = z_entity_log_new();

        game.map = z_entity_map_new(&game, "gfx/level00.png");
        ZCompMap* map = a_entity_requireComponent(game.map, "map");

        int mapWidth, mapHeight;
        z_comp_map_getDim(map, &mapWidth, &mapHeight);

        game.volumeColMap = a_colmap_new(mapWidth * Z_UTIL_TILE_DIM,
                                           mapHeight * Z_UTIL_TILE_DIM,
                                           Z_UTIL_TILE_DIM);

        game.player = z_entity_player_new(&game, 2, 5);

        game.alarm = z_entity_alarm_new(&game);

        z_entity_item_new(&game, Z_ENTITY_ITEM_COFFER, 3, 5);
        z_entity_item_new(&game, Z_ENTITY_ITEM_COFFER, 5, 5);
        z_entity_item_new(&game, Z_ENTITY_ITEM_COFFER, 4, 6);

        z_entity_pigeon_new(&game, 4, 7);
        z_entity_pigeon_new(&game, 6, 7);
        z_entity_pigeon_new(&game, 6, 6);
        z_entity_pigeon_new(&game, 7, 7);
        z_entity_pigeon_new(&game, 12, 12);
        z_entity_pigeon_new(&game, 10, 12);
        z_entity_pigeon_new(&game, 11, 11);
        z_entity_pigeon_new(&game, 10, 5);
        z_entity_pigeon_new(&game, 13, 8);
    }

    A_STATE_BODY
    {
        A_STATE_LOOP
        {
            if(g_returned) {
                g_returned = false;

                if(g_pressedBuy) {
                    g_scamCount++;
                    z_entity_log_write(game.log,
                                       z_util_fonts.red2,
                                       0,
                                       "There's a sucker born every minute  - nice work!");
                } else {
                    z_entity_log_write(game.log,
                                       z_util_fonts.blue2,
                                       0,
                                       "Almost had it!");

                    ZCompAlarm* alarm = a_entity_requireComponent(game.alarm, "alarm");
                    z_comp_alarm_toggle(alarm, true);
                }
            }
        }
    }

    A_STATE_FREE
    {
        a_colmap_free(game.volumeColMap);
    }
}

void z_state_game_getOrigin(const ZStateGame* Game, int* X, int* Y)
{
    ZCompPosition* pos = a_entity_requireComponent(Game->player, "position");

    AFix x, y;
    z_comp_position_getCoords(pos, &x, &y);

    *X = a_fix_fixtoi(x);
    *Y = a_fix_fixtoi(y);
}

AEntity* z_state_game_getLog(const ZStateGame* Game)
{
    return Game->log;
}

AEntity* z_state_game_getMap(const ZStateGame* Game)
{
    return Game->map;
}

AEntity* z_state_game_getPlayer(const ZStateGame* Game)
{
    return Game->player;
}

AEntity* z_state_game_getAlarm(const ZStateGame* Game)
{
    return Game->alarm;
}

AColMap* z_state_game_getVolumeColMap(const ZStateGame* Game)
{
    return Game->volumeColMap;
}

void z_state_game_gameOver(ZStateGame* Game)
{
    A_UNUSED(Game);

    a_system_mute("ttl input ai move animate alarmTick");

    z_entity_log_write(Game->log,
                       z_util_fonts.blue2,
                       0,
                       "People got wise to you");

    z_entity_log_write(Game->log,
                       z_util_fonts.red2,
                       0,
                       "Pulled off %d scams", g_scamCount);

    z_entity_log_write(Game->log,
                       NULL,
                       1,
                       "GAME OVER");
}

void z_state_game_hypnotize(ZStateGame* Game)
{
    A_UNUSED(Game);
    a_state_push("hypnotize");
}

typedef struct ZCircle {
    int x, y;
    int radius;
    APixel color;
    int alpha;
    int alphadec;
} ZCircle;

static ZCircle* circle_new(int X, int Y, APixel Color)
{
    ZCircle* c = a_mem_malloc(sizeof(ZCircle));

    c->x = a_screen_getWidth() / 2 + X - 8 + a_random_getInt(17);
    c->y = a_screen_getHeight() / 2 + Y - 8 + a_random_getInt(17);
    c->radius = 1;
    c->color = Color;
    c->alpha = 256;
    c->alphadec = 1 + a_random_getInt(2);

    return c;
}

A_STATE(hypnotize)
{
    A_STATE_BODY
    {
        g_pressedBuy = false;

        AList* circles = a_list_new();
        bool showBuy = false;

        ATimer* timer1 = a_timer_new(A_TIMER_FRAMES,
                                     a_fps_msToFrames(
                                        a_random_getInRangeu(1000, 4000)));
        a_timer_start(timer1);

        ATimer* timer2 = a_timer_new(A_TIMER_FRAMES, a_fps_msToFrames(500));

        A_STATE_LOOP
        {
            APixel colors[] = {
                z_util_colors.blue1,
                z_util_colors.blue2,
                z_util_colors.red1,
                z_util_colors.red2,
                z_util_colors.gray3,
                z_util_colors.gray4,
            };

            ZCircle* circle = circle_new(0, 0, colors[a_random_getInt(A_ARRAY_LEN(colors))]);
            a_list_addLast(circles, circle);

            A_LIST_ITERATE(circles, ZCircle*, c) {
                c->radius += 2;
                c->alpha = a_math_max(0, c->alpha - c->alphadec);

                if(c->radius > a_screen_getWidth()) {
                    free(c);
                    A_LIST_REMOVE_CURRENT();
                }
            }

            if(a_timer_isExpired(timer1)) {
                showBuy = true;
                a_timer_stop(timer1);
                a_timer_start(timer2);
            }

            if(a_timer_isExpired(timer2)) {
                showBuy = false;
                a_timer_stop(timer2);
                g_returned = true;
                a_state_pop();
            }

            if(a_button_getPressedOnce(z_util_controls.action) && showBuy) {
                g_pressedBuy = true;
                g_returned = true;
                a_state_pop();
            }

            A_STATE_LOOP_DRAW
            {
                A_LIST_ITERATE(circles, ZCircle*, c) {
                    a_pixel_setPixel(c->color);
                    a_pixel_setBlend(A_PIXEL_BLEND_RGBA);
                    a_pixel_setAlpha(c->alpha);
                    a_draw_circle(c->x, c->y, c->radius);
                }

                if(showBuy) {
                    a_sprite_blitCenter(z_util_sprites_getSingle("buy"));
                }
            }
        }

        a_timer_free(timer1);
        a_timer_free(timer2);
        a_list_freeEx(circles, free);
    }
}
