XBE_TITLE = xaudio
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(wildcard $(CURDIR)/*.c)
NXDK_DIR = $(CURDIR)/../..

all: nxdk_wav.h

include $(NXDK_DIR)/Makefile

# nxdk_wav.h: nxdk.wav
# 	ffmpeg -i $< -f s16le -acodec pcm_s16le $@.bin
# 	xxd --include $@.bin $@
