# screenshoter
This project is a cross-platform screenshot tool that captures screenshots on both macOS and Linux systems.
The tool uses platform-specific APIs to capture screenshots and save them as PNG files in the `screenshots` directory.

[![Build and Test on macOS](https://github.com/harou24/screenshoter/actions/workflows/build-macos.yml/badge.svg)](https://github.com/harou24/screenshoter/actions/workflows/build-macos.yml)
[![Build and Test on Ubuntu](https://github.com/harou24/screenshoter/actions/workflows/build-ubuntu.yml/badge.svg)](https://github.com/harou24/screenshoter/actions/workflows/build-ubuntu.yml)

## How to Run

### Prerequisites

#### macOS
- Xcode Command Line Tools (for `clang` and `make`)
- Ensure that your terminal application (iTerm2, Terminal, etc.) has Screen Recording permissions in `System Preferences -> Security & Privacy -> Privacy -> Screen Recording`.

#### Linux
- GCC
- X11 development libraries
- `libpng` development libraries

### Compile the Project
```
make
```

### Run the Program
```
./screenshot
```
The screenshots will be saved in the screenshots directory with filenames in the format screenshot_YYYYMMDDHHMMSS.png.

### Cleaning Up
To remove the compiled executable and object files, run:
```
Make clean
```
