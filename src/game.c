#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>

#include "linked_list.h"
#include "mbstrings.h"
#include "common.h"

/** Updates the game by a single step, and modifies the game information
 * accordingly. Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: width of the board.
 *  - height: height of the board.
 *  - snake_p: pointer to your snake struct (not used until part 3!)
 *  - input: the next input.
 *  - growing: 0 if the snake does not grow on eating, 1 if it does.
 */
void update(int* cells, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing) {
    // `update` should update the board, your snake's data, and global
    // variables representing game information to reflect new state. If in the
    // updated position, the snake runs into a wall or itself, it will not move
    // and global variable g_game_over will be 1. Otherwise, it will be moved
    // to the new position. If the snake eats food, the game score (`g_score`)
    // increases by 1. This function assumes that the board is surrounded by
    // walls, so it does not handle the case where a snake runs off the board.

    // TODO: implement!
    // if g_game_over = 1, game over
    if (g_game_over) {
        return;
    } 
    int current_location = *((int*)snake_p->snake_position->data);
    int snake_new_head_x = current_location % width;
    int snake_new_head_y = current_location / width;

    // if length >= 2 and Reverse immediately, still original direction
    int length_snake = length_list(snake_p->snake_position);
    switch (input) {
        case INPUT_UP:
            if (snake_p->snake_direction == INPUT_DOWN 
                && length_snake >= 2) {
                break;
            }
            snake_p->snake_direction = input;
            break;
        case INPUT_DOWN:
            if (snake_p->snake_direction == INPUT_UP 
                && length_snake >= 2) { 
                break;
            }
            snake_p->snake_direction = input;
            break;
        case INPUT_LEFT:
            if (snake_p->snake_direction == INPUT_RIGHT 
                && length_snake >= 2) { 
                break;
            }
            snake_p->snake_direction = input;
            break;
        case INPUT_RIGHT:
            if (snake_p->snake_direction == INPUT_LEFT
                && length_snake >= 2) { 
                break;
            }
            snake_p->snake_direction = input;
            break;
        default:
            break;
    }

    switch (snake_p->snake_direction) {
        case INPUT_UP:
            snake_new_head_y--;
            break;
        case INPUT_DOWN:
            snake_new_head_y++;
            break;
        case INPUT_LEFT:
            snake_new_head_x--;
            break;
        case INPUT_RIGHT:
            snake_new_head_x++;
            break;
        default:
            break;
    }


    int new_location = snake_new_head_y * width + snake_new_head_x;

    // if next is wall, fail
    if ((cells[new_location] & FLAG_WALL)) {
        g_game_over = 1;
        return;
    }

    // if next is body, fail
    node_t* p = snake_p->snake_position;
    while (p->next) {
        if (new_location == *(int*)p->data) {
            g_game_over = 1;
            return;
        }
        p = p->next;
    }

    insert_first(&(snake_p->snake_position), &new_location, sizeof(int));
    
    //if next is food, food is eaten and score adds
    if ((cells[new_location] & FLAG_FOOD)) {
        place_food(cells, width, height);
        g_score++;
        cells[new_location] ^= FLAG_FOOD; 
        if (growing == 0) {
            // remove the snake from tail location 
            int* old_position = (int*)remove_last(&(snake_p->snake_position));
            cells[*old_position] ^= FLAG_SNAKE;
            free(old_position);
        }   
    } else {
        // remove the snake from tail location 
        int* old_position = (int*)remove_last(&(snake_p->snake_position));
        cells[*old_position] ^= FLAG_SNAKE;
        free(old_position);
    }

    // move to the next location
    // first deal with tail, then deal with head
    cells[new_location] |= FLAG_SNAKE;
}

/** Sets a random space on the given board to food.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: the width of the board
 *  - height: the height of the board
 */
void place_food(int* cells, size_t width, size_t height) {
    /* DO NOT MODIFY THIS FUNCTION */
    unsigned food_index = generate_index(width * height);
    // check that the cell is empty or only contains grass
    if ((*(cells + food_index) == PLAIN_CELL) || (*(cells + food_index) == FLAG_GRASS)) {
        *(cells + food_index) |= FLAG_FOOD;
    } else {
        place_food(cells, width, height);
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Prompts the user for their name and saves it in the given buffer.
 * Arguments:
 *  - `write_into`: a pointer to the buffer to be written into.
 */
void read_name(char* write_into) {
    // TODO: implement! (remove the call to strcpy once you begin your
    // implementation)
    while (true) {
        printf("Name > ");
        fflush(stdin);
        fflush(stdout);
        int bytes = (int)read(0, write_into, 1000);
        if (bytes > 1) {
            write_into[bytes - 1] = '\0';
            break;
        }
        printf("\nName Invalid: must be longer than 0 characters.\n");
    }
}

/** Cleans up on game over — should free any allocated memory so that the
 * LeakSanitizer doesn't complain.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - snake_p: a pointer to your snake struct. (not needed until part 3)
 */
void teardown(int* cells, snake_t* snake_p) {
    // TODO: implement!
    free(cells);
    while (snake_p->snake_position) {
        free(remove_last(&snake_p->snake_position));
    }
}
