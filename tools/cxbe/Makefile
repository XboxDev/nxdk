INCLUDES := $(wildcard *.h)
SRCS := $(wildcard *.cpp)

cxbe: $(SRCS) $(INCLUDES)
	$(CXX) -o '$@' $(SRCS)

.PHONY: clean
clean:
	rm -f cxbe
