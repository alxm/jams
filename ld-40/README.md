# Mine Op 40

Unfinished RTS game prototype for Ludum Dare 40. Jam theme was "The more you have, the worse it is", but I ran out of time before I could implement what happens when the bots mine too many crystals.

Download binaries from [https://alxm.itch.io/mineop40](https://alxm.itch.io/mineop40)

![Screenshot](https://github.com/alxm/mineop40/raw/master/screenshots/MineOp40.gif "Screenshot")

## Controls

* Mouse left-click selects units
* Click on map to dispatch workers
* Select a worker then click on a crystal to start mining
* Press ESC key at any time to quit

## Running

### Windows

* Built for 64bit system
* Launch `mineop40.exe`

### Linux

* Built for 64bit Ubuntu 16.04/Linux Mint 18
* You may need to install these libraries: `libSDL2 libSDL2_mixer libpng12`
* Launch `mineop40`

## Building Source Code

* Install a2x `rel_ld40` from [here](https://github.com/alxm/a2x/tree/rel_ld40)
* Clone this repo, `cd make && make` or `make -f Makefile.mingw64`

## License

Copyright 2017 Alex Margarit (alex@alxm.org)

* Code licensed under [GNU GPL3](https://www.gnu.org/licenses/gpl.html) (see `COPYING`)
* Graphics licensed under [CC BY-NC-ND 4.0](https://creativecommons.org/licenses/by-nc-nd/4.0/) (see `CC-BY-NC-ND`)
