#!/bin/sh
# . dev.sh
geany -i src/* make/* README.md dev.sh &
gimp -n gfx/*.png &
cd make
