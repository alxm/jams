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

typedef struct ZCompTtl ZCompTtl;
typedef void ZCompTtlCallback(AEntity* Entity);

extern size_t z_comp_ttl_size(void);
extern void z_comp_ttl_init(ZCompTtl* Ttl, unsigned ValueMs, ZCompTtlCallback* Callback);

extern unsigned z_comp_ttl_getValue(const ZCompTtl* Ttl);
extern void z_comp_ttl_setValue(ZCompTtl* Ttl, unsigned Value);

extern ZCompTtlCallback* z_comp_ttl_getCallback(const ZCompTtl* Ttl);
