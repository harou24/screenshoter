# Makefile for compiling screenshot program for macOS and Linux

# Define the directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = .
SCREENSHOTS_DIR = screenshots

# Define the source files
MACOS_SRC = $(SRC_DIR)/screenshot_macos.c
LINUX_SRC = $(SRC_DIR)/screenshot_linux.c
MAIN_SRC = $(SRC_DIR)/main.c

# Define the output executable
OUT = $(BIN_DIR)/screenshot

# Detect the operating system
UNAME_S := $(shell uname -s)

# Compiler and flags
CC = gcc
CFLAGS = -Wall -O2

# Platform-specific settings
ifeq ($(UNAME_S), Darwin)
    CC = clang
    LDFLAGS = -framework ApplicationServices -framework CoreGraphics -framework ImageIO
    SRC = $(MACOS_SRC)
    OBJ = $(OBJ_DIR)/screenshot_macos.o $(OBJ_DIR)/main.o
else
    CFLAGS += -lX11 -lpng
    SRC = $(LINUX_SRC)
    OBJ = $(OBJ_DIR)/screenshot_linux.o $(OBJ_DIR)/main.o
endif

.PHONY: all clean $(SCREENSHOTS_DIR)

all: $(SCREENSHOTS_DIR) $(OUT)

$(OUT): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(CFLAGS) -o $(OUT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(MAIN_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SCREENSHOTS_DIR):
	@mkdir -p $(SCREENSHOTS_DIR)

clean:
	rm -rf $(OUT) $(OBJ_DIR)

