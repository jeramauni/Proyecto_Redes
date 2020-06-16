CC=g++
CFLAGS=-g -I. -std=c++11
DEPS = Socket.h Chat.h XLDisplay.h Vector2.h Player.h
OBJ = Socket.o Chat.o XLDisplay.o Vector2.o Player.o
LIBS=-lpthread -lm -lX11 -std=c++11

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: cs cc

cs: $(OBJ) ChatServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) ChatClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o cs cc

const char * XLDisplay::font =  "*-liberation sans-*-r-*-*-*-*";
