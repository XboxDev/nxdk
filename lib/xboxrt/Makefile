XBOXRT_SRCS := \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/fileio.c \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/dirent.c \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/gettimeofday.c \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/strtok_r.c \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/stat.c \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/direct.c \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/strings.c \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/wchar.c \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/wchar_ext_.c \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/wctype_ext_.c \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/stdlib_ext_.c \
	$(NXDK_DIR)/lib/xboxrt/libc_extensions/string_ext_.c \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/_alldiv.s \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/_allmul.s \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/_allrem.s \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/_allshl.s \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/_allshr.s \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/_aulldiv.s \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/_aullrem.s \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/_aullshl.s \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/_aullshr.s \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/_fltused.c \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/check_stack.c \
	$(NXDK_DIR)/lib/xboxrt/c_runtime/chkstk.s \
	$(NXDK_DIR)/lib/xboxrt/vcruntime/purecall.c \
	$(NXDK_DIR)/lib/xboxrt/vcruntime/stack_protection.c \
	$(NXDK_DIR)/lib/xboxrt/vcruntime/threadsafe_statics.c \
	$(NXDK_DIR)/lib/xboxrt/vcruntime/vcruntime_exception.cpp \
	$(NXDK_DIR)/lib/xboxrt/vcruntime/vcruntime_typeinfo.cpp \
	$(NXDK_DIR)/lib/xboxrt/vcruntime/eh.cpp \
	$(NXDK_DIR)/lib/xboxrt/vcruntime/excpt.cpp

XBOXRT_OBJS = $(addsuffix .obj, $(basename $(XBOXRT_SRCS)))

# This file needs to be always built with -fno-rtti because it contains the
# type_info class. If the parameter is missing, clang will generate an invalid
# reference from type_info to its own vftable.
$(NXDK_DIR)/lib/xboxrt/vcruntime/vcruntime_typeinfo.obj: NXDK_CXXFLAGS += -fno-rtti

$(NXDK_DIR)/lib/libxboxrt.lib: $(XBOXRT_OBJS)

# Keep the new libc modules current when their public or shared headers change.
# Explicit paths also avoid MSYS /c/... versus compiler C:/... depfile aliases.
$(NXDK_DIR)/lib/xboxrt/libc_extensions/dirent.obj: $(NXDK_DIR)/lib/xboxrt/libc_extensions/dirent.h $(NXDK_DIR)/lib/xboxrt/libc_extensions/fs_internal.h
$(NXDK_DIR)/lib/xboxrt/libc_extensions/stat.obj: $(NXDK_DIR)/lib/xboxrt/libc_extensions/sys/stat.h $(NXDK_DIR)/lib/xboxrt/libc_extensions/sys/types.h $(NXDK_DIR)/lib/xboxrt/libc_extensions/fs_internal.h
$(NXDK_DIR)/lib/xboxrt/libc_extensions/fileio.obj: $(NXDK_DIR)/lib/xboxrt/libc_extensions/fd_internal.h $(NXDK_DIR)/lib/xboxrt/libc_extensions/fs_internal.h $(NXDK_DIR)/lib/xboxrt/libc_extensions/fcntl.h $(NXDK_DIR)/lib/xboxrt/libc_extensions/unistd.h
$(NXDK_DIR)/lib/xboxrt/libc_extensions/gettimeofday.obj: $(NXDK_DIR)/lib/xboxrt/libc_extensions/sys/time.h
$(NXDK_DIR)/lib/xboxrt/libc_extensions/strtok_r.obj: $(NXDK_DIR)/lib/xboxrt/libc_extensions/string_ext_.h

main.exe: $(NXDK_DIR)/lib/libxboxrt.lib

CLEANRULES += clean-xboxrt

.PHONY: clean-xboxrt
clean-xboxrt:
	$(VE)rm -f $(XBOXRT_OBJS) $(NXDK_DIR)/lib/libxboxrt.lib
