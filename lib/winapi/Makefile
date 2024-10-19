include $(NXDK_DIR)/lib/winapi/winmm/Makefile
include $(NXDK_DIR)/lib/winapi/ws2_32/Makefile

WINAPI_SRCS := \
	$(NXDK_DIR)/lib/winapi/badptr.c \
	$(NXDK_DIR)/lib/winapi/debug.c \
	$(NXDK_DIR)/lib/winapi/errhandlingapi.c \
	$(NXDK_DIR)/lib/winapi/error.c \
	$(NXDK_DIR)/lib/winapi/fiber.c \
	$(NXDK_DIR)/lib/winapi/fileio.c \
	$(NXDK_DIR)/lib/winapi/filemanip.c \
	$(NXDK_DIR)/lib/winapi/findfile.c \
	$(NXDK_DIR)/lib/winapi/handleapi.c \
	$(NXDK_DIR)/lib/winapi/ioapi.c \
	$(NXDK_DIR)/lib/winapi/memory.c \
	$(NXDK_DIR)/lib/winapi/libloaderapi.c \
	$(NXDK_DIR)/lib/winapi/profiling.c \
	$(NXDK_DIR)/lib/winapi/shlobj_core.c \
	$(NXDK_DIR)/lib/winapi/sync.c \
	$(NXDK_DIR)/lib/winapi/sysinfo.c \
	$(NXDK_DIR)/lib/winapi/thread.c \
	${NXDK_DIR}/lib/winapi/timezoneapi.c \
	$(NXDK_DIR)/lib/winapi/tls.c \
	$(NXDK_DIR)/lib/winapi/winnt.c

WINAPI_OBJS = $(addsuffix .obj, $(basename $(WINAPI_SRCS)))

$(NXDK_DIR)/lib/libwinapi.lib: $(WINAPI_OBJS)

main.exe: $(NXDK_DIR)/lib/libwinapi.lib

CLEANRULES += clean-winapi

.PHONY: clean-winapi
clean-winapi:
	$(VE)rm -f $(WINAPI_OBJS) $(NXDK_DIR)/lib/libwinapi.lib
