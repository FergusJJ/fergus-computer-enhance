# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -pedantic
LDFLAGS :=

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Find all .c files in the source directory
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Define the target executable
TARGET := $(BIN_DIR)/hw1

# Default target
all: directories $(TARGET)

# Rule to create necessary directories
directories:
	@mkdir -p $(SRC_DIR) $(OBJ_DIR) $(BIN_DIR)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Rule to compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean the build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) compile_commands.json

# Rule to generate compile_commands.json for clangd LSP
compile_commands.json: Makefile
	@echo '[' > $@
	@for src in $(SRCS); do \
		echo '{"directory":"$(CURDIR)","command":"$(CC) $(CFLAGS) -c $$src -o $(OBJ_DIR)/$$(basename $$src .c).o","file":"$$src"},' >> $@; \
	done
	@sed '$$s/,$$//' $@ > $@.tmp && mv $@.tmp $@
	@echo ']' >> $@

.PHONY: all directories clean
