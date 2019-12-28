/*
    Copyright 2017 Alex Margarit <alex@alxm.org>

    Despot 3900 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Despot 3900 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Despot 3900.  If not, see <http://www.gnu.org/licenses/>.
*/

extern A_STATE(actionMenu);

extern bool z_action_doNothing(ZGame* Game);

extern bool z_action_collectTaxes(ZGame* Game);
extern bool z_action_collectTaxesFromPeasants(ZGame* Game);
extern bool z_action_collectTaxesFromNobles(ZGame* Game);

extern bool z_action_giveMoney(ZGame* Game);
extern bool z_action_giveMoneyToPeasants(ZGame* Game);
extern bool z_action_giveMoneyToNobles(ZGame* Game);

extern bool z_action_imprison(ZGame* Game);
extern bool z_action_imprisonPeasants(ZGame* Game);
extern bool z_action_imprisonNobles(ZGame* Game);

extern bool z_action_wageWar(ZGame* Game);
