CC = g++
CCFLAGS = -Wall -std=c++11
OBJS = TCPConnection.o \
	Server.o \
	Configuration.o \
	IRCServer.o

.PHONY: all clean

all: $(OBJS)
	$(CC) $(CCFLAGS) -o main main.cpp $(OBJS)

clean:
	rm -f `find . -name "*.o"`

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $^
