CC = gcc
FLAGS = -ggdb3 -Wall -Wextra -Wshadow -std=gnu11 -Wno-unused-parameter -Wno-unused-but-set-variable -Werror -fsigned-char

# Linking ncurses works differently on Linux and Mac. Detect
# OS to account for this
HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
ifeq ($(SYSTEM),Darwin)
LIBS = -lncurses
FLAGS += -D_XOPEN_SOURCE_EXTENDED
else
LIBS = $(shell ncursesw5-config --libs)
FLAGS += $(shell ncursesw5-config --cflags)
endif

FILES = $(wildcard src/*.c) $(wildcard src/*.h)
OBJS = src/game.o src/game_setup.o src/render.o src/common.o src/linked_list.o src/mbstrings.o src/game_over.o
BIN = snake

all: $(BIN)

# wildcard rule for compiling object file from source and header
src/%.o: src/%.c src/%.h
	$(CC) $(FLAGS) -c $< -o $@

$(BIN): $(OBJS) src/snake.c
	$(CC) $(FLAGS) $^ $(LIBS) -o $@ -lm

format:
	clang-format -style=file -i $(FILES)

clean:
	rm -f $(BIN)
	rm -f ${OBJS)

.PHONY: all clean format