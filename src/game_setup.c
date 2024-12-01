#include "game_setup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "game.h"
#include "common.h"

// Some handy dandy macros for decompression
#define E_CAP_HEX 0x45
#define E_LOW_HEX 0x65
#define G_CAP_HEX 0x47
#define G_LOW_HEX 0x67
#define S_CAP_HEX 0x53
#define S_LOW_HEX 0x73
#define W_CAP_HEX 0x57
#define W_LOW_HEX 0x77
#define DIGIT_START 0x30
#define DIGIT_END 0x39

/** Initializes the board with walls around the edge of the board.
 *
 * Modifies values pointed to by cells_p, width_p, and height_p and initializes
 * cells array to reflect this default board.
 *
 * Returns INIT_SUCCESS to indicate that it was successful.
 *
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 */
enum board_init_status initialize_default_board(int** cells_p, size_t* width_p,
                                                size_t* height_p) {
    *width_p = 20;
    *height_p = 10;
    int* cells = malloc(20 * 10 * sizeof(int));
    *cells_p = cells;
    for (int i = 0; i < 20 * 10; i++) {
        cells[i] = PLAIN_CELL;
    }

    // Set edge cells!
    // Top and bottom edges:
    for (int i = 0; i < 20; ++i) {
        cells[i] = FLAG_WALL;
        cells[i + (20 * (10 - 1))] = FLAG_WALL;
    }
    // Left and right edges:
    for (int i = 0; i < 10; ++i) {
        cells[i * 20] = FLAG_WALL;
        cells[i * 20 + 20 - 1] = FLAG_WALL;
    }

    // Set grass cells!
    // Top and bottom edges:
    for (int i = 1; i < 19; ++i) {
        cells[i + 20] = FLAG_GRASS;
        cells[i + (20 * (9 - 1))] = FLAG_GRASS;
    }
    // Left and right edges:
    for (int i = 1; i < 9; ++i) {
        cells[i * 20 + 1] = FLAG_GRASS;
        cells[i * 20 + 19 - 1] = FLAG_GRASS;
    }

    // Add snake
    cells[20 * 2 + 2] = FLAG_SNAKE;

    return INIT_SUCCESS;
}

/** Initialize variables relevant to the game board.
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 *  - snake_p: a pointer to your snake struct (not used until part 3!)
 *  - board_rep: a string representing the initial board. May be NULL for
 * default board.
 */
enum board_init_status initialize_game(int** cells_p, size_t* width_p,
                                       size_t* height_p, snake_t* snake_p,
                                       char* board_rep) {
    // TODO: implement!
    g_score = 0;
    g_game_over = 0;
    snake_p->snake_position = NULL;
    snake_p->snake_direction = INPUT_RIGHT;

    enum board_init_status initBoardStatus;
    if (board_rep == NULL) {
        initBoardStatus = initialize_default_board(cells_p, width_p, height_p);
        int initial_snake_position = 20 * 2 + 2;
        insert_first(&(snake_p->snake_position), &initial_snake_position, sizeof(int));
    } else {
        initBoardStatus = decompress_board_str(cells_p, width_p, height_p, snake_p, board_rep);
    }
    
    if (initBoardStatus == INIT_SUCCESS) {
        place_food(*cells_p, *width_p, *height_p);
    }
    return initBoardStatus;
}

/** Takes in a string `compressed` and initializes values pointed to by
 * cells_p, width_p, and height_p accordingly. Arguments:
 *      - cells_p: a pointer to the pointer representing the cells array
 *                 that we would like to initialize.
 *      - width_p: a pointer to the width variable we'd like to initialize.
 *      - height_p: a pointer to the height variable we'd like to initialize.
 *      - snake_p: a pointer to your snake struct (not used until part 3!)
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B24x80|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row
 * (delineated by the `|` character), and read out a letter (E, S or W) a number
 * of times dictated by the number that follows the letter.
 */
enum board_init_status decompress_board_str(int** cells_p, size_t* width_p,
                                            size_t* height_p, snake_t* snake_p,
                                            char* compressed) {
    // TODO: implement!
    *cells_p = NULL;
    
    // get the actual height
    int actual_height = 0, k = 0;
    while (compressed[k] != '\0') {
        if (compressed[k] == '|') {
            actual_height++;
        }
        k++;
    }

    // get the desired height and width
    const char* delim = "x|";
    char* token = strtok(compressed, delim);
    if (token[0] != 'B') {
        return INIT_ERR_BAD_CHAR;
    }
    token++;
    *height_p = atoi(token);
    if ((int)*height_p != actual_height) {
        return INIT_ERR_INCORRECT_DIMENSIONS;
    }
    token = strtok(NULL, delim);
    *width_p = atoi(token);
    if (*width_p == 0) {
        return INIT_ERR_BAD_CHAR;
    }

    *cells_p = (int*)malloc(sizeof(int) * (*width_p) * (*height_p));
    // deal with each row
    int has_snake = 0;
    for (int i = 0; i < (int)*height_p; i++) {
        int current_column = 0;
        int token_index = 0;
        token = strtok(NULL, delim);
        while (token[token_index] != '\0') {
            // deal with each pair in a row
            char current_alpha = token[token_index];
            if (!is_valid_alphabet(current_alpha)) {
                return INIT_ERR_BAD_CHAR;
            }
            token_index++;

            char* alpha_num_char = (char*)malloc(sizeof(char) * 5);
            int alpha_index = 0;
            while (is_valid_number(token[token_index])) {
                alpha_num_char[alpha_index] = token[token_index];
                token_index++;
                alpha_index++;
            }
            int alpha_num = atoi(alpha_num_char);
            free(alpha_num_char);

            if (current_column + alpha_num > (int)*width_p) {
                return INIT_ERR_INCORRECT_DIMENSIONS;
            }
            if (current_alpha == 'S') {
                if (has_snake + alpha_num > 1) {
                    return INIT_ERR_WRONG_SNAKE_NUM;
                } else {
                    has_snake += alpha_num;
                }
            }
            
            initialize(current_alpha, alpha_num, i, current_column, cells_p, width_p, height_p, snake_p);
            current_column += alpha_num;
        }
        if (current_column != (int)*width_p) {
            return INIT_ERR_INCORRECT_DIMENSIONS;
        }
    }
    if (has_snake != 1) {
        return INIT_ERR_WRONG_SNAKE_NUM;
    }

    return INIT_SUCCESS;
}

int is_valid_alphabet(char alpha) {
    if (alpha == 'W' || alpha == 'E' || alpha == 'S' || alpha == 'G') {
        return 1;
    }
    return 0;
}

int is_valid_number(char num) {
    if (num - '0' >= 0 && num - '0' <= 9) {
        return 1;
    }
    return 0;
}

void initialize(char alpha, int num, int row, int column, int** cells_p,
                size_t* width_p, size_t* height_p, snake_t* snake_p) {
    int* cells = *cells_p;
    int current_location = row * *width_p + column;
    if (alpha == 'W') {
        for (int i = 0; i < num; ++i) {
            cells[current_location + i] = FLAG_WALL;
        }
    }
    if (alpha == 'E') {
        for (int i = 0; i < num; ++i) {
            cells[current_location + i] = PLAIN_CELL;
        }
    }
    if (alpha == 'G') {
        for (int i = 0; i < num; ++i) {
            cells[current_location + i] = FLAG_GRASS;
        }
    }
    if (alpha == 'S') {
        cells[current_location] = FLAG_SNAKE;
        int initialize_snake_position = row * (*width_p) + column;
        insert_first(&(snake_p->snake_position), &initialize_snake_position, sizeof(int));
    }
}