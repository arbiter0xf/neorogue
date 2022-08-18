SRC = src/main.cpp
CC = g++
COMPILER_FLAGS = -w
LINKER_FLAGS = -lSDL2 -lSDL2_image
EXE_NAME = rogue_forever

all: $(SRC)
	$(CC) $(SRC) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXE_NAME)

run: all
	LD_LIBRARY_PATH=/usr/local/lib ./$(EXE_NAME)
