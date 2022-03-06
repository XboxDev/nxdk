MAIN = udplogger
CFLAGS = -Wall -g

SRCS = main.c

ifeq (,$(findstring MINGW,$(UNAME_S)))
LDFLAGS = -lws2_32
endif

OBJS = $(SRCS:.c=.o)

$(MAIN): $(OBJS)
	$(CC) -o '$@' $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o '$@' '$<'

.PHONY: clean
clean:
	rm -f $(OBJS)

.PHONY: distclean
distclean: clean
	rm -f $(MAIN)
