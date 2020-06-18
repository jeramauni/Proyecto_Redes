CC=g++
CFLAGS=-g -I. -std=c++11
DEPS = Socket.h CanarIOServer.h CanarIOClient.h Message.h XLDisplay.h Vector2.h Player.h
OBJ = Socket.o CanarIOServer.o CanarIOClient.o Message.o XLDisplay.o Vector2.o Player.o
LIBS=-lpthread -lm -lX11 -std=c++11

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: clean server client 

server: $(OBJ) CanarIOServerApp.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

client: $(OBJ) CanarIOClientApp.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o server client

const char * XLDisplay::font =  "*-liberation sans-*-r-*-*-*-*";
