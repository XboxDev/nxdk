SDL_TTF_DIR = $(NXDK_DIR)/lib/sdl/SDL_ttf
SDL_TTF_SRCS += $(SDL_TTF_DIR)/SDL_ttf.c
SDL_TTF_OBJS = $(addsuffix .obj, $(basename $(SDL_TTF_SRCS)))

$(NXDK_DIR)/lib/libSDL_ttf.lib: $(SDL_TTF_OBJS)

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
FREETYPE_OBJS = $(addsuffix .obj, $(basename $(FREETYPE_SRCS)))

$(NXDK_DIR)/lib/libfreetype.lib: $(FREETYPE_OBJS)

CFLAGS += -I$(NXDK_DIR)/lib/sdl \
          -I$(SDL_TTF_DIR) \
          -I$(FREETYPE_DIR)/include \
          -DXBOX

CXXFLAGS += -I$(NXDK_DIR)/lib/sdl \
          -I$(SDL_TTF_DIR) \
          -I$(FREETYPE_DIR)/include \
          -DXBOX

main.exe: $(NXDK_DIR)/lib/libSDL_ttf.lib $(NXDK_DIR)/lib/libfreetype.lib

CLEANRULES += clean-sdl_ttf

.PHONY: clean-sdl_ttf
clean-sdl_ttf:
	$(VE)rm -f $(SDL_TTF_OBJS) \
	           $(FREETYPE_OBJS) \
	           $(NXDK_DIR)/lib/libSDL_ttf.lib \
	           $(NXDK_DIR)/lib/libfreetype.lib
