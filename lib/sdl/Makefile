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
FREETYPE_SRCS += $(FREETYPE_DIR)/src/smooth/smooth.c
FREETYPE_SRCS += $(FREETYPE_DIR)/src/autofit/autofit.c
FREETYPE_SRCS += $(FREETYPE_DIR)/src/sfnt/sfnt.c

SRCS += $(SDL_TTF_SRCS) $(FREETYPE_SRCS)
CFLAGS += -I$(NXDK_DIR)/lib/sdl \
          -I$(SDL_TTF_DIR) \
          -I$(FREETYPE_DIR)/include \
          -DXBOX
