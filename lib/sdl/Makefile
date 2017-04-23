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

SRCS += $(SDL_SRCS)
CFLAGS += -I$(SDL_DIR)/include -DXBOX
