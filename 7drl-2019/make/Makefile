A_CONFIG_APP_AUTHOR := alxm
A_CONFIG_APP_BIN := city.bin
A_CONFIG_APP_TITLE := City

A_CONFIG_APP_VERSION_MAJOR := 1
A_CONFIG_APP_VERSION_MINOR := 0
A_CONFIG_APP_VERSION_MICRO := 0

#A_CONFIG_SCREEN_WIDTH := -1
#A_CONFIG_SCREEN_HEIGHT := -1
A_CONFIG_SCREEN_WIDTH := 320
A_CONFIG_SCREEN_HEIGHT := 200
#A_CONFIG_SCREEN_MAXIMIZED := 0
#A_CONFIG_SCREEN_ZOOM := 4

include $(A2X_PATH)/make/dev

Z_GFX_PAL_PY := ../assets/gfx/pal.py
Z_GFX_PAL_PNG := ../assets/gfx/palette.png
Z_GFX_PNG_FILES := $(wildcard ../assets/tiles/*.png) $(wildcard ../assets/actors/*.png)

gfx :
	python3 $(Z_GFX_PAL_PY) $(Z_GFX_PAL_PNG) $(Z_GFX_PNG_FILES)
