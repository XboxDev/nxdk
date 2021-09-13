INCLUDES := \
	Common.h \
	Cxbx.h \
	Error.h \
	Exe.h \
	Xbe.h
SRCS := \
	Main.cpp \
	Common.cpp \
	Error.cpp \
	Exe.cpp \
	Logo.cpp \
	Xbe.cpp

cxbe: $(SRCS) $(INCLUDES)
	$(CXX) -o '$@' $(SRCS)

.PHONY: clean
clean:
	rm -f cxbe
