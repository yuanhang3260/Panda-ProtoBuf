CC=g++ -std=c++11
CFLAGS=-Wall -Werror -O2
LFLAGS=-lssl -lcrypto -pthread
IFLAGS=-I../src/

OBJ=tiny_pb.o simple_pb.o

all: test server client

tiny_pb.o: tiny_pb.cpp tiny_pb.h
	$(CC) $(CFLAGS) $(IFLAGS) -c tiny_pb.cpp

test1.o: test1_pb.cpp test1_pb.h
	$(CC) $(CFLAGS) $(IFLAGS) -c test1_pb.cpp

simple_pb.o: simple_pb.cpp simple_pb.h
	$(CC) $(CFLAGS) $(IFLAGS) -c simple_pb.cpp

ProtoBuf_main.o: ProtoBuf_main.cpp
	$(CC) $(CFLAGS) $(IFLAGS) -c ProtoBuf_main.cpp

test: $(OBJ) $(OBJ) ProtoBuf_main.o ../libfull.a
	$(CC) $(CFLAGS) $(OBJ) ProtoBuf_main.o ../libfull.a -o $@

RpcServer_main.o: RpcServer_main.cpp
	$(CC) $(CFLAGS) $(IFLAGS) -c RpcServer_main.cpp

server: $(OBJ) simple_pb.o RpcServer_main.o ../libfull.a
	$(CC) $(CFLAGS) $(LFLAGS) simple_pb.o RpcServer_main.o ../libfull.a -o $@

RpcClient_main.o: RpcClient_main.cpp
	$(CC) $(CFLAGS) $(IFLAGS) -c RpcClient_main.cpp

client: $(OBJ) simple_pb.o RpcClient_main.o ../libfull.a
	$(CC) $(CFLAGS) $(LFLAGS) simple_pb.o RpcClient_main.o ../libfull.a -o $@

clean:
	rm -rf *.o
	rm -rf test client server
