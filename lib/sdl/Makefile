# Include SDL sources
SDL_DIR = $(NXDK_DIR)/lib/sdl/SDL2
SDL_SRCS += $(wildcard $(SDL_DIR)/src/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/cpuinfo/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/events/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/file/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/render/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/render/software/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/stdlib/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/thread/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/thread/generic/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/timer/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/timer/dummy/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/video/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/video/xbox/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/libm/*.c)
SDL_SRCS += $(wildcard $(SDL_DIR)/src/atomic/*.c)

SDL_TTF_DIR = $(NXDK_DIR)/lib/sdl/SDL_ttf
SDL_TTF_SRCS += $(SDL_TTF_DIR)/SDL_ttf.c

FREETYPE_DIR = $(NXDK_DIR)/lib/sdl/SDL_ttf/external/freetype-2.4.12
FREETYPE_SRCS += $(FREETYPE_DIR)/src/base/ftbase.c
FREETYPE_SRCS += $(FREETYPE_DIR)/src/base/ftglyph.c
FREETYPE_SRCS += $(FREETYPE_DIR)/src/base/ftbitmap.c
FREETYPE_SRCS += $(FREETYPE_DIR)/src/base/ftstroke.c
FREETYPE_SRCS += $(FREETYPE_DIR)/src/base/ftinit.c
FREETYPE_SRCS += $(FREETYPE_DIR)/src/base/ftsystem.c
FREETYPE_SRCS += $(FREETYPE_DIR)/src/truetype/truetype.c
FREETYPE_SRCS += $(FREETYPE_DIR)/src/raster/raster.c
FREETYPE_SRCS += $(FREETYPE_DIR)/src/autofit/autofit.c

SRCS += $(SDL_SRCS) $(SDL_TTF_SRCS) $(FREETYPE_SRCS)
CFLAGS += -I$(NXDK_DIR)/lib/sdl \
          -I$(SDL_DIR)/include \
          -I$(SDL_TTF_DIR) \
          -I$(FREETYPE_DIR)/include \
          -DXBOX
