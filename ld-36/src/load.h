/*
    Copyright 2016 Alex Margarit

    This file is part of Rushed Dinosaur Dare.

    Rushed Dinosaur Dare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Rushed Dinosaur Dare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rushed Dinosaur Dare.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <a2x.h>

typedef struct GlobalData {
    ASpriteFrames* boneGraphics;
    ASpriteFrames* grassGraphics;
    ASpriteFrames* buggyBackTracks;
    ASpriteFrames* buggyFrontTracks;
    ASprite* buggyChassis;
    ASprite* buggyCabin;
    ASprite* stegoBody;
    ASpriteFrames* stegoLegs;
    AInput* up;
    AInput* down;
    AInput* left;
    AInput* right;
    AInput* jump;
    AInput* shoot;
    int fontCyan;
    int fontMagenta;
} GlobalData;

extern GlobalData g_data;

extern A_STATE(load);
