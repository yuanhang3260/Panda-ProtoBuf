CC=g++ -std=c++11
CFLAGS=-Wall -Werror -O2
LFLAGS=-lssl -lcrypto -pthread
IFLAGS=-I../src/ -I../src/Public/

HYLIB=../../HyLib/libhy.a

OBJ=tiny_pb.o simple_pb.o

all: test server client

tiny_pb.o: tiny_pb.cc tiny_pb.h
	$(CC) $(CFLAGS) $(IFLAGS) -c tiny_pb.cc

test1.o: test1_pb.cc test1_pb.h
	$(CC) $(CFLAGS) $(IFLAGS) -c test1_pb.cc

simple_pb.o: simple_pb.cc simple_pb.h
	$(CC) $(CFLAGS) $(IFLAGS) -c simple_pb.cc

ProtoBuf_main.o: ProtoBuf_main.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c ProtoBuf_main.cc

test: $(OBJ) ProtoBuf_main.o ../libfull.a $(HYLIB)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJ) ProtoBuf_main.o ../libfull.a $(HYLIB) -o $@

RpcServer_main.o: RpcServer_main.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c RpcServer_main.cc

server: $(OBJ) simple_pb.o RpcServer_main.o ../libfull.a $(HYLIB)
	$(CC) $(CFLAGS) $(LFLAGS) simple_pb.o RpcServer_main.o ../libfull.a $(HYLIB) -o $@

RpcClient_main.o: RpcClient_main.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c RpcClient_main.cc

client: $(OBJ) simple_pb.o RpcClient_main.o ../libfull.a $(HYLIB)
	$(CC) $(CFLAGS) $(LFLAGS) simple_pb.o RpcClient_main.o ../libfull.a $(HYLIB) -o $@

clean:
	rm -rf *.o
	rm -rf test client server

fullclean:
	rm -rf *.o
	rm -rf *_pb.h
	rm -rf *_pb.cc
	rm -rf test client server	
