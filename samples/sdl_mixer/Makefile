XBE_TITLE = nxdk\ sample\ -\ SDL2_mixer
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(CURDIR)/main.c
NXDK_DIR ?= $(CURDIR)/../..
NXDK_SDL = y
NXDK_SDL_MIXER = y

include $(NXDK_DIR)/Makefile

TARGET += $(OUTPUT_DIR)/sdl_mixer_tone.wav \
          $(OUTPUT_DIR)/sdl_mixer_music.wav

$(GEN_XISO): $(OUTPUT_DIR)/sdl_mixer_tone.wav $(OUTPUT_DIR)/sdl_mixer_music.wav

$(OUTPUT_DIR)/sdl_mixer_tone.wav: $(CURDIR)/sdl_mixer_tone.wav $(OUTPUT_DIR)
	$(VE)cp '$<' '$@'

$(OUTPUT_DIR)/sdl_mixer_music.wav: $(CURDIR)/sdl_mixer_music.wav $(OUTPUT_DIR)
	$(VE)cp '$<' '$@'
