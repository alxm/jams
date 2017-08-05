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

typedef struct ZCompInput ZCompInput;
typedef struct ZCompInputBinding ZCompInputBinding;
typedef void ZCompInputHandler(AEntity* Entity);

extern size_t z_comp_input_size(void);
extern void z_comp_input_init(ZCompInput* Input);
extern AComponentFree z_comp_input_free;

extern AList* z_comp_input_getBindings(const ZCompInput* Input);
extern AInputButton* z_comp_input_bindingGetButton(const ZCompInputBinding* Binding);
extern ZCompInputHandler* z_comp_input_bindingGetHandler(const ZCompInputBinding* Binding);

extern void z_comp_input_addBinding(ZCompInput* Input, AInputButton* Button, ZCompInputHandler* Handler);
