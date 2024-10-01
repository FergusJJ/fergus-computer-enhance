# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -pedantic -I./include

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target executable
TARGET := $(BIN_DIR)/main

# Default target
all: $(TARGET) compile_commands.json

# Linking the target executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@

# Compiling object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Generate compile_commands.json for LSP
compile_commands.json: $(SRCS)
	@echo "[" > $@
	@for src in $(SRCS); do \
		echo "  {" >> $@; \
		echo "    \"directory\": \"$(CURDIR)\"," >> $@; \
		echo "    \"command\": \"$(CC) $(CFLAGS) -c $$src -o $(OBJ_DIR)/$$(basename $$src .c).o\"," >> $@; \
		echo "    \"file\": \"$$src\"" >> $@; \
		echo "  }," >> $@; \
	done
	@sed -i '$$s/,$$//' $@
	@echo "]" >> $@

# Cleaning up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) compile_commands.json

# Phony targets
.PHONY: all clean
