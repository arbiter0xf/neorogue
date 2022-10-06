SRC_GAME_DIR := src
HEADERS_GAME_DIR := include
SRC_TEST_DIR := test

SRC_GAME := $(shell find $(SRC_GAME_DIR)/ -name "*.cpp")
HEADERS_GAME := $(shell find $(HEADERS_GAME_DIR)/ -name "*.hpp")
SRC_TEST := \
	    $(filter-out $(SRC_GAME_DIR)/main.cpp, $(SRC_GAME)) \
	    $(shell find $(SRC_TEST_DIR)/ -name "*.cpp")

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
INCLUDES_TEST := -I/opt/gtest/include/
COMMON_COMPILER_FLAGS := $(INCLUDES) -w -std=c++17
COMPILER_FLAGS_GAME := \
		       $(COMMON_COMPILER_FLAGS) \
		       -DGRAPHICS_ENABLED=1
COMPILER_FLAGS_TEST := \
		      $(INCLUDES_TEST) \
		      $(COMMON_COMPILER_FLAGS) \
		      -pthread \
		      -DBUILD_FOR_TESTS=1
LINKER_FLAGS = -lSDL2 -lSDL2_image
LINKER_FLAGS_TEST := \
		    $(LINKER_FLAGS) \
		    -lgtest \
		    -L/opt/gtest/lib
EXE_NAME_GAME := rogue_forever
EXE_NAME_MAP_EDITOR := rogue_forever_map_editor
EXE_NAME_TEST := rogue_forever_test

ALL_EXE_NAMES := \
		 $(EXE_NAME_GAME) \
		 $(EXE_NAME_MAP_EDITOR) \
		 $(EXE_NAME_TEST)

all: $(ALL_EXE_NAMES)

$(EXE_NAME_GAME): $(SRC_GAME) $(HEADERS_GAME_DIR)
	$(CXX) $(SRC_GAME) $(COMPILER_FLAGS_GAME) $(LINKER_FLAGS) -o $@

$(EXE_NAME_MAP_EDITOR): $(SRC_MAP_EDITOR)
	@echo "Map editor build placeholder"

$(EXE_NAME_TEST): $(SRC_TEST)
	$(CXX) $^ $(COMPILER_FLAGS_TEST) $(LINKER_FLAGS_TEST) -o $@

targets:
	@echo $(ALL_EXE_NAMES)

run: $(EXE_NAME_GAME)
	LD_LIBRARY_PATH=/usr/local/lib ./$(EXE_NAME_GAME)

test: $(EXE_NAME_TEST)
	LD_LIBRARY_PATH=/usr/local/lib ./$(EXE_NAME_TEST)
