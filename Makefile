# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = -lm

# Directories
SRC_DIR = src
BUILD_DIR = build

# SDL3 configuration
SDL3_CFLAGS = $(shell pkg-config --cflags sdl3)
SDL3_LDFLAGS = $(shell pkg-config --libs sdl3)

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target executable
TARGET = $(BUILD_DIR)/doomclone

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(SDL3_LDFLAGS)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SDL3_CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Rebuild everything
rebuild: clean all

# Run the executable
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean rebuild run

