/*
    Copyright 2020 Alex Margarit <alex@alxm.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "u_sound.h"
#include "main.h"

#define U_NOTES_NUM 6

static FSample* g_notes[U_NOTES_NUM];
static FTimer* g_timer;

void u_sound_init(void)
{
    for(int i = 0; i < U_NOTES_NUM; i++) {
        char buffer[64];
        f_str_fmt(buffer, sizeof(buffer), false, "assets/sfx/n%d.wav", i + 1);

        g_notes[i] = f_sample_new(buffer);
    }

    g_timer = f_timer_new(F_TIMER_MS, 1000, true);

    f_timer_runStart(g_timer);
}

void u_sound_uninit(void)
{
    for(int i = 0; i < U_NOTES_NUM; i++) {
        f_sample_free(g_notes[i]);
    }

    f_timer_free(g_timer);
}

void u_sound_tick(void)
{
    if(f_timer_expiredGet(g_timer)) {
        static const unsigned periods[] = {
            250, 500, 500, 1000, 1000, 1000, 1500, 1500, 2000
        };

        f_timer_expiredClear(g_timer);
        f_timer_periodSet(
            g_timer, periods[f_random_intu(F_ARRAY_LEN(periods))]);
        f_timer_runStart(g_timer);

        f_channel_playStart(F_CHANNEL_ANY,
                            g_notes[f_random_intu(U_NOTES_NUM)],
                            F_CHANNEL_PLAY_NORMAL);
    }
}
