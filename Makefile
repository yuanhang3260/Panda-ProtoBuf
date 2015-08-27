#
# Snoopy Protocol Buffer
#
# Hang Yuan <yuanhang3260@gmail.com>
#
CC=g++ -std=c++11
CFLAGS=-Wall -Werror -O2
LFLAGS=-lssl -lcrypto -pthread

SRC_DIR=src
OBJ_DIR=lib
VPATH=$(SOURCE)
      
OBJ = $(OBJ_DIR)/Utility/BufferedDataReader.o \
      $(OBJ_DIR)/Utility/BufferedDataWriter.o \
      $(OBJ_DIR)/Utility/Strings.o \
      $(OBJ_DIR)/Utility/StringBuilder.o \
      $(OBJ_DIR)/Utility/Utils.o \
      $(OBJ_DIR)/IO/FileDescriptorInterface.o \
      $(OBJ_DIR)/IO/FileDescriptor.o \
      $(OBJ_DIR)/IO/TextPrinter.o \
      $(OBJ_DIR)/Log/Log.o \
      $(OBJ_DIR)/Network/Socket.o \
      
COMPILER_OBJ = \
      $(OBJ_DIR)/Compiler/ProtoParser.o \
      $(OBJ_DIR)/Compiler/CppCodeGenerator.o \
      $(OBJ_DIR)/Compiler/PbCommon.o \
      $(OBJ_DIR)/Compiler/Message.o  \
      $(OBJ_DIR)/Compiler/MessageField.o \
      $(OBJ_DIR)/Compiler/EnumType.o \
      $(OBJ_DIR)/Compiler/Type.o \
      
PROTO_OBJ = \
			$(OBJ_DIR)/Proto/MessageReflection.o \
			$(OBJ_DIR)/Proto/MessageFactory.o \
			$(OBJ_DIR)/Proto/SerializedPrimitive.o \
			$(OBJ_DIR)/Proto/SerializedMessage.o \
			$(OBJ_DIR)/Proto/WireFormat.o \

TESTOBJ = $(OBJ_DIR)/IO/TextPrinter_test.o \
          $(OBJ_DIR)/Utility/StringBuilder_test.o \
          $(OBJ_DIR)/Utility/Strings_test.o \
          $(OBJ_DIR)/Proto/RepeatedField_test.o \

TESTEXE = test/TextPrinter_test.out \
					test/StringBuilder_test.out \
					test/Strings_test.out \
					test/RepeatedField_test.out \

COMPILEROBJ = $(OBJ_DIR)/Compiler/CppCompiler_main.o

default: library compiler

test: $(TESTEXE) library

library: $(OBJ) $(COMPILER_OBJ) $(PROTO_OBJ)
	ar cr libsnp.a $(OBJ) $(COMPILER_OBJ) $(PROTO_OBJ)

compiler: $(SRC_DIR)/Compiler/Compiler_main.cpp library
	$(CC) $(CFLAGS) $(LFLAGS) -c $(SRC_DIR)/Compiler/Compiler_main.cpp -o $(COMPILEROBJ)
	$(CC) $(CFLAGS) $(LFLAGS) $(COMPILEROBJ) libsnp.a -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Utility/%.o: $(SRC_DIR)/Utility/%.cpp $(SRC_DIR)/Utility/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Utility/%.o: $(SRC_DIR)/Utility/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/IO/%.o: $(SRC_DIR)/IO/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Network/%.o: $(SRC_DIR)/Network/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Compiler/%.o: $(SRC_DIR)/Compiler/%.cpp $(SRC_DIR)/Compiler/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Proto/%.o: $(SRC_DIR)/Proto/%.cpp $(SRC_DIR)/Proto/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Proto/%.o: $(SRC_DIR)/Proto/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

test/%.out: $(OBJ_DIR)/Utility/%.o library
	$(CC) $(CFLAGS) $(LFLAGS) $< libsnp.a -o $@

test/%.out: $(OBJ_DIR)/Proto/%.o library
	$(CC) $(CFLAGS) $(LFLAGS) $< libsnp.a -o $@

# test/%.out: $(OBJ_DIR)/Network/%.o library
# 	$(CC) $(CFLAGS) $(LFLAGS) $< libsnp.a -lssl -lcrypto -o $@

test/%.out: $(OBJ_DIR)/IO/%.o library
	$(CC) $(CFLAGS) $(LFLAGS) $< libsnp.a -lssl -lcrypto -o $@

# test/%.out: test/%.o library
# 	$(CC) $(CFLAGS) $(LFLAGS) $< libsnp.a -o $@

clean:
	rm -rf libsnp.a
	rm -rf compiler
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(OBJ_DIR)/Utility/*.o
	rm -rf $(OBJ_DIR)/IO/*.o
	rm -rf $(OBJ_DIR)/Network/*.o
	rm -rf $(OBJ_DIR)/Compiler/*.o
	rm -rf $(OBJ_DIR)/Log/*.o
	rm -rf $(OBJ_DIR)/Proto/*.o
	rm -rf test/*.out
	rm -rf samples/*.o
	rm -rf *.output

tinyclean:
	rm -rf libsnp.a
	rm -rf compiler
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(OBJ_DIR)/Compiler/*.o
	rm -rf test/*.out
	rm -rf *.output

sampleclean:
	rm -rf samples/*.o

