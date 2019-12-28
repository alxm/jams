#!/bin/sh
# . dev.sh
geany -i src/* make/* dev.sh README.md &
gimp -n assets/gfx/tiles.png &
cd make
