#ifndef GAME_H
#define GAME_H

#include <stddef.h>

#include "common.h"

void read_name(char* write_into);
void read_status(char* write_into);
void read_board(char* write_info);
void read_txt(char* filename, char** board);
void output_rank();
void update(int* cells, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing);
void place_food(int* cells, size_t width, size_t height);
void teardown(int* cells, snake_t* snake_p, char* board);

#endif
