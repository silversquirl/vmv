.POSIX:
.PHONY: all clean

CC := c99

CMACROS :=

ifdef NO_POSIX
CMACROS += NO_POSIX
else
CMACROS += _POSIX_C_SOURCE=200809L
endif

ifdef STATIC
CMACROS += GLEW_STATIC
endif

CFLAGS := -Wall -Werror $(patsubst %,-D%,$(CMACROS))
LDFLAGS := -lfftw3 -lm -lGL -lGLEW -lglfw

all: visualiser

clean:
	rm -f visualiser
	rm -f *.o

visualiser: visualiser.o audio.o graphics.o timer.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
