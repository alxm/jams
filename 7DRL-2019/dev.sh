#!/bin/sh
geany -i assets/gfx/*.py assets/tiles/*.txt make/* src/* dev.sh README.md &
gimp -n assets/actors/*.png assets/gfx/*.png assets/tiles/*.png &
cd make
