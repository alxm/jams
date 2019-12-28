#!/usr/bin/env python3

'''
    Copyright 2019 Alex Margarit <alex@alxm.org>
    This file is part of City, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''

import sys
from PIL import Image

class Palette:
    def __init__(self, PaletteFile):
        self.diff = {}

        image = Image.open(PaletteFile).convert('RGB')
        width, height = image.size

        if width < 1 or height != 2:
            print('Invalid palette {}'.format(PaletteFile))
            sys.exit(1)

        pixels = image.load()

        for x in range(width):
            oldColor = pixels[x, 0]
            newColor = pixels[x, 1]

            if oldColor != newColor:
                self.diff[oldColor] = newColor

        print('{} palette diffs'.format(len(self.diff)))

        if len(self.diff) == 0:
            sys.exit(0)

        for x in range(width):
            pixels[x, 0] = pixels[x, 1]

        print('Updating {}'.format(PaletteFile))
        image.save(PaletteFile)

def main(PaletteFile, ImageFiles):
    palette = Palette(PaletteFile)

    for imageFile in ImageFiles:
        image = Image.open(imageFile).convert('RGB')
        width, height = image.size
        pixels = image.load()
        dirty = False

        for y in range(height):
            for x in range(width):
                p = pixels[x, y]

                if p in palette.diff:
                    pixels[x, y] = palette.diff[p]
                    dirty = True

        if dirty:
            print('Updating {}'.format(imageFile))
            image.save(imageFile)

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Usage: {} Palette.png Image1.png ...'.format(sys.argv[0]))
        sys.exit(1)
    else:
        main(sys.argv[1], sys.argv[2 :])
