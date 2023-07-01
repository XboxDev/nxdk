INCLUDES := \
	Common.h \
	Cxbx.h \
	Error.h \
	Exe.h \
	Xbe.h

COMMON_SRCS := \
	Common.cpp \
	Error.cpp \
	Exe.cpp \
	Logo.cpp \
	Xbe.cpp

CXBE_SRCS := Main.cpp $(COMMON_SRCS)
cxbe: $(CXBE_SRCS) $(INCLUDES)
	$(CXX) -o '$@' $(CXBE_SRCS)

DUMPXBE_SRCS := DumpMain.cpp $(COMMON_SRCS)
dumpxbe: $(DUMPXBE_SRCS) $(INCLUDES)
	$(CXX) -o '$@' $(DUMPXBE_SRCS)

.PHONY: all
all: cxbe dumpxbe

.PHONY: clean
clean:
	rm -f cxbe
	rm -f dumpxbe
