#!/bin/sh
# . dev.sh
geany -i src/* make/* assets/maps/cave/*.txt dev.sh README.md &
gimp -n assets/gfx/*.png assets/maps/cave/*.png &
cd make
