CC=g++
CFLAGS=-g -I. -std=c++11
DEPS = XLDisplay.h Vector2.h Player.h
OBJ = CanarIO-server.o XLDisplay.o Player.o
LIBS=-lpthread -lm -lX11 -std=c++11

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: CanarIO-server Player Vector2

CanarIO-server: $(OBJ) CanarIO-server.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o CanarIO-server

const char * XLDisplay::font =  "*-liberation sans-*-r-*-*-*-*";