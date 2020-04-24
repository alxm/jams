F_CONFIG_APP_AUTHOR := alxm
F_CONFIG_APP_NAME := dream46

F_CONFIG_SCREEN_SIZE_WIDTH := 800
F_CONFIG_SCREEN_SIZE_HEIGHT := 480

F_CONFIG_COLOR_SCREEN_BORDER := 0xa2a0ab

F_CONFIG_PATH_EMBED_EMSCRIPTEN := \
	assets/gfx/font_6x8.png \
	assets/sfx/n1.wav \
	assets/sfx/n2.wav \
	assets/sfx/n3.wav \
	assets/sfx/n4.wav \
	assets/sfx/n5.wav \
	assets/sfx/n6.wav \
	assets/sfx/hiss.wav \
	assets/sfx/jingle.wav \

include $(FAUR_PATH)/make/emscripten.mk
