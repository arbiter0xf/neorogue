SRC_GAME_DIR := src/game
SRC_PUGIXML_DIR := external/pugixml/src
HEADERS_PUGIXML_DIR := external/pugixml/include
HEADERS_GAME_DIR := include
SRC_TEST_DIR := test

SRC_GAME := \
	    $(shell find $(SRC_GAME_DIR)/ -name "*.cpp") \
	    $(shell find $(SRC_PUGIXML_DIR)/ -name "*.cpp")
HEADERS_GAME := $(shell find $(HEADERS_GAME_DIR)/ -name "*.hpp")
SRC_TEST := \
	    $(filter-out $(SRC_GAME_DIR)/main.cpp, $(SRC_GAME)) \
	    $(shell find $(SRC_TEST_DIR)/ -name "*.cpp")

SRC_SERVER_ROUTE := src/server_route/main.cpp
SRC_SERVER_ROUTE_TEST_CLIENT := src/server_route/test_client.cpp

CC := g++
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
	    -Iexternal/boostorg/asio/include/ \
	    -Iexternal/boostorg/date_time/include/ \
	    -Iexternal/boostorg/smart_ptr/include/ \
	    -Iexternal/boostorg/utility/include/ \
	    -Iexternal/boostorg/type_traits/include/ \
	    -Iexternal/boostorg/numeric_conversion/include/ \
	    -Iexternal/boostorg/mpl/include/ \
	    -Iexternal/boostorg/preprocessor/include/ \
	    -Iexternal/boostorg/bind/include/ \
	    -Iexternal/boostorg/regex/include/ \
	    -Iexternal/boostorg/predef/include/ \
	    -I$(HEADERS_PUGIXML_DIR) \
	    -Iinclude
INCLUDES_TEST := -I/opt/gtest/include/
COMMON_COMPILER_FLAGS := $(INCLUDES) -w -std=c++20
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
LINKER_FLAGS = -lSDL2 -lSDL2_image
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
