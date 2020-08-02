ifeq ($(OS), Windows_NT)
	PLATTFORM = WINDOWS
	CLEAR_COMMAND = del
	ADDITIONAL_LIB = -lws2_32
else
	PLATTFORM = LINUX
	CLEAR_COMMAND = rm
endif

INCLUDE_DIR = -Iinclude
LIB_FILE = lib/$(PLATTFORM)-NetLib.a
BIN-INT = bin-int/
DEFINES = -D$(PLATTFORM) -DDEBUG
EXECUTABLE = -o Result
C_FLAGS = -g
CC = g++

all: Main Net
	$(CC) $(C_FLAGS) $(EXECUTABLE) $(BIN-INT)Main.o $(LIB_FILE) $(INCLUDE_DIR) $(DEFINES) $(ADDITIONAL_LIB)
	$(CC) $(C_FLAGS) -o Server.exe examples/Server.cpp $(LIB_FILE) $(INCLUDE_DIR) $(DEFINES) $(ADDITIONAL_LIB)
	$(CC) $(C_FLAGS) -o Client.exe examples/Client.cpp $(LIB_FILE) $(INCLUDE_DIR) $(DEFINES) $(ADDITIONAL_LIB)

Main: Main.cpp
	$(CC) $(C_FLAGS) -c -o $(BIN-INT)Main.o Main.cpp $(INCLUDE_DIR) $(DEFINES)

Net: net/net.cpp
	$(CC) $(C_FLAGS) -c -o $(BIN-INT)net.o net/net.cpp $(INCLUDE_DIR) $(DEFINES)
	ar rcs $(LIB_FILE) $(BIN-INT)net.o