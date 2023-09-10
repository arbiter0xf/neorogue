CC = gcc

CFLAGS := -Wall -Werror -pedantic -std=c17

EXE_GAME_CLIENT := rogue_forever
EXE_UTEST := utest

SRC_UTEST := $(wildcard test/*.c)

OBJ_UTEST := $(SRC_UTEST:.c=.o)

CHECK_FRAMEWORK_LIBS_FLAGS := $(shell pkg-config --libs check)

$(EXE_UTEST): $(OBJ_UTEST)
	$(CC) $(CFLAGS) -o $(EXE_UTEST) $^ $(CHECK_FRAMEWORK_LIBS_FLAGS)

$(EXE_GAME_CLIENT):
	echo "Not yet implemented"

clean:
	$(RM) \
		$(EXE_UTEST) \
		$(EXE_GAME_CLIENT) \
		$(OBJ_UTEST)
