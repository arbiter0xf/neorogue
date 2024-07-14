SRC_GAME_DIR := src/game
HEADERS_GAME_DIR := include
SRC_TEST_DIR := test

SRC_GAME := \
	    $(shell find $(SRC_GAME_DIR)/ -name "*.cpp")
HEADERS_GAME := $(shell find $(HEADERS_GAME_DIR)/ -name "*.hpp")
SRC_TEST := \
	    $(filter-out $(SRC_GAME_DIR)/main.cpp, $(SRC_GAME)) \
	    $(shell find $(SRC_TEST_DIR)/ -name "*.cpp")

SRC_SERVER_ROUTE := src/server_route/main.cpp
SRC_SERVER_ROUTE_TEST_CLIENT := src/server_route/test_client.cpp

CC := g++
INCLUDES := -Iinclude
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
COMPILER_FLAGS_SERVER_ROUTE := \
			       $(COMMON_COMPILER_FLAGS)
COMPILER_FLAGS_SERVER_ROUTE_TEST_CLIENT := $(COMPILER_FLAGS_SERVER_ROUTE)
LINKER_FLAGS = -lSDL2 -lSDL2_image -lcjson
LINKER_FLAGS_TEST := \
		    $(LINKER_FLAGS) \
		    -lgtest \
		    -L/opt/gtest/lib
EXE_NAME_GAME := rogue_forever
EXE_NAME_SERVER_ROUTE := rogue_forever_server_route
EXE_NAME_SERVER_ROUTE_TEST_CLIENT := rogue_forever_server_route_test_client
EXE_NAME_TEST := rogue_forever_test

ALL_EXE_NAMES := \
		 $(EXE_NAME_GAME) \
		 $(EXE_NAME_TEST)

all: $(ALL_EXE_NAMES)

$(EXE_NAME_GAME): $(SRC_GAME) $(HEADERS_GAME_DIR)
	$(CXX) $(SRC_GAME) $(COMPILER_FLAGS_GAME) $(LINKER_FLAGS) -o $@

$(EXE_NAME_SERVER_ROUTE): $(SRC_SERVER_ROUTE)
	$(CXX) $(SRC_SERVER_ROUTE) $(COMPILER_FLAGS_SERVER_ROUTE) -o $@

$(EXE_NAME_SERVER_ROUTE_TEST_CLIENT): $(SRC_SERVER_ROUTE_TEST_CLIENT)
	$(CXX) $(SRC_SERVER_ROUTE_TEST_CLIENT) $(COMPILER_FLAGS_SERVER_ROUTE_TEST_CLIENT) -o $@

$(EXE_NAME_TEST): $(SRC_TEST)
	$(CXX) $^ $(COMPILER_FLAGS_TEST) $(LINKER_FLAGS_TEST) -o $@

targets:
	@echo $(ALL_EXE_NAMES)

run: $(EXE_NAME_GAME)
	LD_LIBRARY_PATH=/usr/local/lib ./$(EXE_NAME_GAME)

test: $(EXE_NAME_TEST)
	LD_LIBRARY_PATH=/usr/local/lib ./$(EXE_NAME_TEST)
