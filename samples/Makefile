CC=g++ -std=c++11
CFLAGS=-Wall -Werror -O2
LFLAGS=-lssl -lcrypto -pthread
IFLAGS=-I../src/

OBJ=tiny.o

all: $(OBJ)

tiny.o: tiny_pb.cpp tiny_pb.h
	$(CC) $(CFLAGS) $(IFLAGS) -c tiny_pb.cpp
