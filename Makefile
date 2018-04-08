.POSIX:
.PHONY: all clean

CC := c99

CFLAGS := -Wall -Werror
LDFLAGS := -lfftw3 -lm

all: visualiser

clean:
	rm -f visualiser
	rm -f *.o

visualiser: visualiser.o audio.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
