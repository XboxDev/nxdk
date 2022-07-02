FMT_DIR = $(NXDK_DIR)/lib/fmt/fmt

FMT_SRCS = $(FMT_DIR)/src/format.cc \
           $(FMT_DIR)/src/os.cc

FMT_OBJS = $(addsuffix .obj, $(basename $(FMT_SRCS)))

NXDK_CXXFLAGS += -I$(FMT_DIR)/include -I$(FMT_DIR)/src

$(NXDK_DIR)/lib/fmt.lib: $(FMT_OBJS)

main.exe: $(NXDK_DIR)/lib/fmt.lib

CLEANRULES += clean-fmt

.PHONY: clean-fmt
clean-fmt:
	$(VE)rm -f $(FMT_OBJS) $(NXDK_DIR)/lib/fmt.lib
