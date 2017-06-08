#
# Protocol Buffer
#
# Hang Yuan <yuanhang3260@gmail.com>
#
CC=g++ -std=c++11
CFLAGS=-Wall -Werror -O2
LFLAGS=-lssl -lcrypto -pthread
IFLAGS=-Isrc/ -Isrc/Public/

SRC_DIR=src
OBJ_DIR=lib
VPATH=$(SOURCE)

HYLIB_DIR=../HyLib/
HYLIB=../HyLib/libhy.a
      
COMPILER_OBJ = \
      $(OBJ_DIR)/Compiler/ProtoParser.o \
      $(OBJ_DIR)/Compiler/CppCodeGenerator.o \
      $(OBJ_DIR)/Compiler/PbCommon.o \
      $(OBJ_DIR)/Compiler/Message.o  \
      $(OBJ_DIR)/Compiler/ServiceType.o  \
      $(OBJ_DIR)/Compiler/MessageField.o \
      $(OBJ_DIR)/Compiler/EnumType.o \
      $(OBJ_DIR)/Compiler/Type.o \
      
PROTO_OBJ = \
      $(OBJ_DIR)/Proto/Common.o \
      $(OBJ_DIR)/Proto/MessageReflection.o \
      $(OBJ_DIR)/Proto/MessageFactory.o \
      $(OBJ_DIR)/Proto/SerializedPrimitive.o \
      $(OBJ_DIR)/Proto/SerializedMessage.o \
      $(OBJ_DIR)/Proto/WireFormat.o \
      $(OBJ_DIR)/Proto/Message.o \
      $(OBJ_DIR)/Proto/Descriptor.o \
      $(OBJ_DIR)/Proto/Descriptors_internal.o \
      $(OBJ_DIR)/Proto/DescriptorsBuilder.o \

RPC_OBJ = \
      $(OBJ_DIR)/RPC/Rpc.o \
      $(OBJ_DIR)/RPC/RpcService.o \
      $(OBJ_DIR)/RPC/RpcServer.o \
      $(OBJ_DIR)/RPC/RpcCommon.o \
      $(OBJ_DIR)/RPC/RpcChannelBase.o \
      $(OBJ_DIR)/RPC/RpcClientChannel.o \
      $(OBJ_DIR)/RPC/RpcServerChannel.o \
      $(OBJ_DIR)/RPC/RpcPacket_pb.o \

TESTOBJ = $(OBJ_DIR)/Proto/RepeatedField_test.o \
          $(OBJ_DIR)/Proto/DescriptorsBuilder_test.o \
          $(OBJ_DIR)/Proto/MessageReflection_test.o \
          

TESTEXE = test/RepeatedField_test.out \
					test/DescriptorsBuilder_test.out \
					test/MessageReflection_test.out \

COMPILEROBJ = $(OBJ_DIR)/Compiler/Compiler_main.o

default: pre_build full

pre_build:
	mkdir -p $(OBJ_DIR)/Compiler $(OBJ_DIR)/Proto $(OBJ_DIR)/RPC

full: libhy proto_library library compiler

libhy:
	+$(MAKE) -C $(HYLIB_DIR)

proto: proto_library compiler

test: $(TESTEXE) proto_library

# basic proto and compiler library
proto_library: $(COMPILER_OBJ) $(PROTO_OBJ)
	ar cr libproto.a $(COMPILER_OBJ) $(PROTO_OBJ)

# full library contains RPC
library: $(COMPILER_OBJ) $(PROTO_OBJ) $(RPC_OBJ)
	ar cr libfull.a $(COMPILER_OBJ) $(PROTO_OBJ) $(RPC_OBJ)

compiler: $(SRC_DIR)/Compiler/Compiler_main.cc proto_library $(HYLIB)
	$(CC) $(CFLAGS) $(IFLAGS) -c $(SRC_DIR)/Compiler/Compiler_main.cc -o $(COMPILEROBJ)
	$(CC) $(CFLAGS) $(LFLAGS) $(COMPILEROBJ) libproto.a $(HYLIB) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Compiler/%.o: $(SRC_DIR)/Compiler/%.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Compiler/%.o: $(SRC_DIR)/Compiler/%.cc $(SRC_DIR)/Compiler/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Proto/%.o: $(SRC_DIR)/Proto/%.cc $(SRC_DIR)/Proto/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Proto/%.o: $(SRC_DIR)/Proto/%.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/RPC/%.o: $(SRC_DIR)/RPC/%.cc $(SRC_DIR)/RPC/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

test/%.out: $(OBJ_DIR)/Utility/%.o proto_library
	$(CC) $(CFLAGS) $(LFLAGS) $< libproto.a -o $@

test/%.out: $(OBJ_DIR)/Proto/%.o proto_library
	$(CC) $(CFLAGS) $(LFLAGS) $< libproto.a -o $@

test/%.out: $(OBJ_DIR)/IO/%.o proto_library
	$(CC) $(CFLAGS) $(LFLAGS) $< libproto.a -lssl -lcrypto -o $@


clean:
	rm -rf libproto.a
	rm -rf libfull.a
	rm -rf compiler
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(OBJ_DIR)/Compiler/*.o
	rm -rf $(OBJ_DIR)/Proto/*.o
	rm -rf $(OBJ_DIR)/RPC/*.o
	rm -rf test/*.out
	rm -rf samples/*.o
	rm -rf samples/*_pb.h
	rm -rf samples/*_pb.cc
	rm -rf samples/client
	rm -rf samples/server
	rm -rf samples/test
	rm -rf *.output

tinyclean:
	rm -rf libproto.a
	rm -rf libfull.a
	rm -rf compiler
	rm -rf $(OBJ_DIR)/Compiler/*.o
	rm -rf $(OBJ_DIR)/Proto/*.o
	rm -rf $(OBJ_DIR)/RPC/*.o
	rm -rf test/*.out
	rm -rf samples/*.o
	rm -rf samples/*_pb.h
	rm -rf samples/*_pb.cc
	rm -rf samples/client
	rm -rf samples/server
	rm -rf samples/test
	rm -rf *.output

fullclean: clean
	rm -rf samples/*_pb.h
	rm -rf samples/*_pb.cc
	
sampleclean:
	rm -rf samples/*.o

