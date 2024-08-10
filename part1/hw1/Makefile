# Define the compiler and flags
CC = gcc
CFLAGS = -Wall
LDFLAGS =

# Find the first .c file in the current directory
SRC = $(wildcard *.c)
ifeq ($(words $(SRC)),0)
$(error No .c files found in the current directory)
endif
SRC := $(firstword $(SRC))

# Define the target executable based on the source file name
TARGET = $(basename $(SRC))

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Rule to clean the build artifacts
clean:
	rm -f $(TARGET) compile_commands.json

# Rule to generate compile_commands.json for clangd LSP
compile_commands.json: Makefile
	echo '[{"directory":"$(CURDIR)","command":"$(CC) $(CFLAGS) -c $(SRC)","file":"$(SRC)"}]' > $@

.PHONY: all clean
