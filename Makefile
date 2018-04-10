.POSIX:
.PHONY: all clean

CFLAGS := -Wall -Werror
LDFLAGS := -lm

include config.mk

ifdef NO_POSIX
CFLAGS += -DNO_POSIX
else
CFLAGS += -D_POSIX_C_SOURCE=200809L
endif

ifdef STATIC_GLEW
CFLAGS += -DGLEW_STATIC
endif

ifdef STATIC_GLFW
LDFLAGS += -lX11 -lpthread -ldl
endif

ifdef DEBUG
CFLAGS += -DDEBUG
endif

all: visualiser

clean:
	rm -f visualiser
	rm -f *.o

visualiser: visualiser.o audio.o graphics.o timer.o lua_api.o lua_config.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
