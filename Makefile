#
# Protocol Buffer
#
# Hang Yuan <yuanhang3260@gmail.com>
#
CC=g++ -std=c++11
CFLAGS=-Wall -Werror -O2
LFLAGS=-lssl -lcrypto -pthread
IFLAGS=-Isrc/

SRC_DIR=src
OBJ_DIR=lib
VPATH=$(SOURCE)
      
OBJ = $(OBJ_DIR)/Utility/BufferedDataReader.o \
      $(OBJ_DIR)/Utility/BufferedDataWriter.o \
      $(OBJ_DIR)/Utility/Strings.o \
      $(OBJ_DIR)/Utility/StringBuilder.o \
      $(OBJ_DIR)/Utility/Utils.o \
      $(OBJ_DIR)/Utility/ThreadPool.o \
      $(OBJ_DIR)/Utility/EventManager.o \
      $(OBJ_DIR)/IO/FileDescriptorInterface.o \
      $(OBJ_DIR)/IO/FileDescriptor.o \
      $(OBJ_DIR)/IO/FileUtils.o \
      $(OBJ_DIR)/IO/TextPrinter.o \
      $(OBJ_DIR)/Log/Log.o \
      $(OBJ_DIR)/Network/Socket.o \
      
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
			$(OBJ_DIR)/Proto/MessageReflection.o \
			$(OBJ_DIR)/Proto/MessageDescriptor.o \
			$(OBJ_DIR)/Proto/MessageFactory.o \
			$(OBJ_DIR)/Proto/SerializedPrimitive.o \
			$(OBJ_DIR)/Proto/SerializedMessage.o \
			$(OBJ_DIR)/Proto/WireFormat.o \

RPC_OBJ = \
      $(OBJ_DIR)/RPC/Rpc.o \
      $(OBJ_DIR)/RPC/RpcService.o \
      $(OBJ_DIR)/RPC/RpcServer.o \
      $(OBJ_DIR)/RPC/RpcCommon.o \
      $(OBJ_DIR)/RPC/RpcChannelBase.o \
      $(OBJ_DIR)/RPC/RpcClientChannel.o \
      $(OBJ_DIR)/RPC/RpcServerChannel.o \
      $(OBJ_DIR)/RPC/RpcSession_pb.o \

TESTOBJ = $(OBJ_DIR)/IO/TextPrinter_test.o \
          $(OBJ_DIR)/Utility/StringBuilder_test.o \
          $(OBJ_DIR)/Utility/Strings_test.o \
          $(OBJ_DIR)/Proto/RepeatedField_test.o \

TESTEXE = test/TextPrinter_test.out \
					test/StringBuilder_test.out \
					test/Strings_test.out \
					test/RepeatedField_test.out \

COMPILEROBJ = $(OBJ_DIR)/Compiler/ccCompiler_main.o

default: full

full: proto_library library compiler

proto: proto_library compiler

test: $(TESTEXE) proto_library library

proto_library: $(OBJ) $(COMPILER_OBJ) $(PROTO_OBJ)
	ar cr libproto.a $(OBJ) $(COMPILER_OBJ) $(PROTO_OBJ)

library: $(OBJ) $(COMPILER_OBJ) $(PROTO_OBJ) $(RPC_OBJ)
	ar cr libfull.a $(OBJ) $(COMPILER_OBJ) $(PROTO_OBJ) $(RPC_OBJ)

compiler: $(SRC_DIR)/Compiler/Compiler_main.cc proto_library
	$(CC) $(CFLAGS) $(LFLAGS) -c $(SRC_DIR)/Compiler/Compiler_main.cc -o $(COMPILEROBJ)
	$(CC) $(CFLAGS) $(LFLAGS) $(COMPILEROBJ) libproto.a -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Utility/%.o: $(SRC_DIR)/Utility/%.cc $(SRC_DIR)/Utility/%.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Utility/%.o: $(SRC_DIR)/Utility/%.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/IO/%.o: $(SRC_DIR)/IO/%.cc
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/Network/%.o: $(SRC_DIR)/Network/%.cc
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
	rm -rf $(OBJ_DIR)/Utility/*.o
	rm -rf $(OBJ_DIR)/IO/*.o
	rm -rf $(OBJ_DIR)/Network/*.o
	rm -rf $(OBJ_DIR)/Compiler/*.o
	rm -rf $(OBJ_DIR)/Log/*.o
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

