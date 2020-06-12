CC=g++
CFLAGS=-g -I. -std=c++11
DEPS = XLDisplay.h
OBJ = CanarIO-client.o XLDisplay.o
LIBS=-lpthread -lm -lX11 -std=c++11

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: CanarIO-client

CanarIO-client: $(OBJ) CanarIO-client.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o CanarIO-client

const char * XLDisplay::font =  "*-liberation sans-*-r-*-*-*-*";