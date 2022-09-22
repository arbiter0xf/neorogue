SRC_GAME := \
       src/main.cpp \
       src/Tile.cpp \
       src/Spritesheet.cpp \
       src/Sdlw.cpp \
       src/Json.cpp \
       src/Log.cpp

SRC_MAP_EDITOR := src/Log.cpp

CC = g++
INCLUDES := \
	    -Iexternal/boostorg/json/include/ \
	    -Iexternal/boostorg/config/include/ \
	    -Iexternal/boostorg/assert/include/ \
	    -Iexternal/boostorg/throw_exception/include/ \
	    -Iexternal/boostorg/core/include/ \
	    -Iexternal/boostorg/container/include/ \
	    -Iexternal/boostorg/move/include/ \
	    -Iexternal/boostorg/static_assert/include/ \
	    -Iexternal/boostorg/intrusive/include/ \
	    -Iexternal/boostorg/system/include/ \
	    -Iexternal/boostorg/mp11/include/ \
	    -Iexternal/boostorg/align/include/ \
	    -Iinclude
COMPILER_FLAGS = $(INCLUDES) -w -std=c++17
LINKER_FLAGS = -lSDL2 -lSDL2_image
EXE_NAME_GAME := rogue_forever
EXE_NAME_MAP_EDITOR := rogue_forever_map_editor

ALL_EXE_NAMES := $(EXE_NAME_GAME) $(EXE_NAME_MAP_EDITOR)

all: $(ALL_EXE_NAMES)

$(EXE_NAME_GAME): $(SRC_GAME)
	$(CC) $^ $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@

$(EXE_NAME_MAP_EDITOR): $(SRC_MAP_EDITOR)
	@echo "Map editor build placeholder"

test: all
	@echo "Test placeholder"

targets:
	@echo $(ALL_EXE_NAMES)

run: all
	LD_LIBRARY_PATH=/usr/local/lib ./$(EXE_NAME)
