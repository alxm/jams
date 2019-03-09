#!/bin/sh
geany -i src/* make/* assets/tiles/*.txt dev.sh README.md &
gimp -n assets/tiles/*.png &
cd make
