#!/bin/sh
# . dev.sh
geany -i src/* make/* assets/maps/cave/*.txt assets/maps/forest/*.txt dev.sh README.md &
gimp -n assets/gfx/*.png assets/maps/cave/*.png assets/maps/forest/*.png &
cd make
