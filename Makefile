# Makefile for compiling screenshot program for macOS and Linux

# Define the source files
COMMON_SRC = common.c
MACOS_SRC = screenshot_macos.c
LINUX_SRC = screenshot_linux.c
MAIN_SRC = main.c

# Define the output executable
OUT = screenshot

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
    SRC = $(COMMON_SRC) $(MACOS_SRC) $(MAIN_SRC)
else
    # Linux settings
    CFLAGS += -lX11 -lpng
    SRC = $(COMMON_SRC) $(LINUX_SRC) $(MAIN_SRC)
endif

.PHONY: all clean

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

clean:
	rm -f $(OUT)

