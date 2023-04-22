SRC_DIR = src
BUILD_DIR = build/debug
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_NAME = gameOfLife
INCLUDE_PATHS =  -I include
LIBRARY_PATHS = -L /opt/homebrew/Cellar/sdl2/2.26.5/lib -L /opt/homebrew/Cellar/sdl2_image/2.6.3/lib -L /opt/homebrew/Cellar/sdl2_ttf/2.20.2/lib
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g
LINKER_FLAGS =  -lsdl2_ttf -lsdl2

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)

