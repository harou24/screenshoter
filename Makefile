# Makefile for compiling screenshot program for macOS and Linux

# Define the directories
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj
BIN_DIR = .
SCREENSHOTS_DIR = screenshots

# Define the source files
COMMON_SRC = $(LIB_DIR)/common.c
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
LIBS =

# OS-specific settings
ifeq ($(UNAME_S), Darwin)
    # macOS settings
    CC = clang
    CFLAGS += -framework ApplicationServices -framework CoreGraphics -framework ImageIO
    SRC = $(MACOS_SRC)
else
    # Linux settings
    CFLAGS += -lX11 -lpng
    SRC = $(LINUX_SRC)
endif

# Object files
OBJ = $(OBJ_DIR)/common.o $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC)) $(OBJ_DIR)/main.o

.PHONY: all clean $(SCREENSHOTS_DIR)

all: $(SCREENSHOTS_DIR) $(OUT)

$(OUT): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(OUT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/common.o: $(COMMON_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(MAIN_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SCREENSHOTS_DIR):
	@mkdir -p $(SCREENSHOTS_DIR)

clean:
	rm -rf $(OUT) $(OBJ_DIR)

