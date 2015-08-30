CC=g++ -std=c++11
CFLAGS=-Wall -Werror -O2
LFLAGS=-lssl -lcrypto -pthread
IFLAGS=-I../src/

OBJ=tiny_pb.o

all: main.out

tiny_pb.o: tiny_pb.cpp tiny_pb.h
	$(CC) $(CFLAGS) $(IFLAGS) -c tiny_pb.cpp

test1.o: test1_pb.cpp test1_pb.h
	$(CC) $(CFLAGS) $(IFLAGS) -c test1_pb.cpp

simple_pb.o: simple_pb.cpp simple_pb.h
	$(CC) $(CFLAGS) $(IFLAGS) -c simple_pb.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) $(IFLAGS) -c main.cpp

main.out: $(OBJ) main.o ../libsnp.a
	$(CC) $(CFLAGS) $(OBJ) main.o ../libsnp.a -o $@

clean:
	rm -rf *.o
