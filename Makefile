CFLAGS=$(shell sdl-config --cflags) -Wall -g
LDFLAGS=$(shell sdl-config --libs)

bouncing-square: bouncing-square.c

clean:
	rm bouncing-square
