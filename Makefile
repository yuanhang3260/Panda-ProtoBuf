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
      $(OBJ_DIR)/IO/FileDescriptorInterface.o \
      $(OBJ_DIR)/IO/FileDescriptor.o \
      $(OBJ_DIR)/IO/TextPrinter.o \
      $(OBJ_DIR)/Network/Socket.o \

TESTOBJ = $(OBJ_DIR)/IO/TextPrinter_test.o \

TESTEXE = test/TextPrinter_test.out \

CLIENTOBJ = $(OBJ_DIR)/HttpClient_main.o

SERVEROBJ = $(OBJ_DIR)/HttpServer_main.o

default: library

test: $(TESTEXE) library

library: $(OBJ)
	ar cr libsnp.a $(OBJ)

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

# test/%.out: $(OBJ_DIR)/Utility/%.o library
# 	$(CC) $(CFLAGS) $(LFLAGS) $< libsnp.a -o $@

# test/%.out: $(OBJ_DIR)/Network/%.o library
# 	$(CC) $(CFLAGS) $(LFLAGS) $< libsnp.a -lssl -lcrypto -o $@

test/%.out: $(OBJ_DIR)/IO/%.o library
	$(CC) $(CFLAGS) $(LFLAGS) $< libsnp.a -lssl -lcrypto -o $@

# test/%.out: test/%.o library
# 	$(CC) $(CFLAGS) $(LFLAGS) $< libsnp.a -o $@

clean:
	rm -rf libsnp.a
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(OBJ_DIR)/Utility/*.o
	rm -rf $(OBJ_DIR)/IO/*.o
	rm -rf $(OBJ_DIR)/Network/*.o
	rm -rf test/*.out
	rm -rf *.output

