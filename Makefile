# Specify the C compiler
CC = clang

# Specify the C compiler flags
CFLAGS = -I/usr/local/Cellar/hidapi/0.14.0/include/hidapi 
CFLAGS += -I/usr/local/Cellar/libserialport/0.1.1/include/libserialport

# Specify the linker flags
LDFLAGS = -L/usr/local/Cellar/hidapi/0.14.0/lib
LDFLAGS += -L/usr/local/Cellar/libserialport/0.1.1/lib

# Specify the libraries to link against
LIBS = -lhidapi -lserialport

# Specify the source file
SRC = keyboard-server.c

# Specify the output file
OUT = keyboard-server

# Define the build target
build:
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $(SRC) -o $(OUT)

# Define the run target
run:
	./$(OUT)

# Define the clean target
clean:
	rm -f $(OUT)
