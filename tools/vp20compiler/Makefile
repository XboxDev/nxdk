MAIN = vp20compiler

INCLUDES = $(wildcard *.h)

SRCS = nvvertparse.c \
	prog_instruction.c \
	main.c

OBJS = $(SRCS:.c=.o)

CFLAGS = -std=gnu99

$(MAIN): $(OBJS)
	$(CC) -o '$@' $(OBJS)

%.o: %.c ${INCLUDES}
	$(CC) $(CFLAGS) -c -o '$@' '$<'

.PHONY: clean
clean:
	rm -f $(OBJS)

.PHONY: distclean
distclean: clean
	rm -f $(MAIN)
