#ifndef RENDER_H
#define RENDER_H

#include <stddef.h>

#include "common.h"

void check_terminal_size(size_t width, size_t height);
void initialize_window(size_t width, size_t height);
void over_game(int* cells, size_t width, size_t height, snake_t* snake_p, char* board);
void end_game(size_t width, size_t height);
void render_game(int* cells, size_t width, size_t height);

#endif
