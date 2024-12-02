#define _XOPEN_SOURCE_EXTENDED 1
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

#include "game.h"
#include "game_over.h"
#include "game_setup.h"
#include "mbstrings.h"
#include "render.h"
#include "common.h"

void read_txt(char* filename, char** board) {
    FILE *file;
    long fileLen;

    file = fopen(filename, "rb");
    
    fseek(file, 0, SEEK_END);
    fileLen = ftell(file);
    rewind(file);

    *board = (char *)malloc(fileLen + 1);

    fread(*board, fileLen, 1, file);
    (*board)[fileLen] = '\0'; 

    fclose(file);
}

void output_rank() {
    FILE *file;    
    char filename[] = "output/rank.txt"; 

    file = fopen(filename, "w");

    char label[100];
    sprintf(label, "%-6s%s%10s", "Rank", "Name", "Score");
    fprintf(file, "%s", label);
    for (int i = 0; i < scores.size; i++) {
        fprintf(file, "\n");
        char out[100];
        sprintf(out, "%-6d%s%10d", i + 1, scores.items->data, scores.items->priority);  
        fprintf(file, "%s", out);     
    }

    fclose(file);
}

/** Gets the next input from the user, or returns INPUT_NONE if no input is
 * provided quickly enough.
 */
enum input_key get_input() {
    /* DO NOT MODIFY THIS FUNCTION */
    int input = getch();

    if (input == KEY_UP) {
        return INPUT_UP;
    } else if (input == KEY_DOWN) {
        return INPUT_DOWN;
    } else if (input == KEY_LEFT) {
        return INPUT_LEFT;
    } else if (input == KEY_RIGHT) {
        return INPUT_RIGHT;
    } else if (input == KEY_F(1)) {
        return INPUT_SPEEDUP;
    } else if (input == KEY_F(2)) {
        return INPUT_SPEEDDOWN;
    } else if (input == ' ') {
        return INPUT_PAUSE;
    } else {
        // if the input isn't an arrow key, we treat it as no input (could add
        // other keys in if we want other commands, like 'pause' or 'quit')
        return INPUT_NONE;
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Helper function that procs the GAME OVER screen and final key prompt.
 * `snake_p` is not needed until Part 3!
 */
void over_game(int* cells, size_t width, size_t height, snake_t* snake_p, char* board) {
    // Game over!

    // Free any memory we've taken
    teardown(cells, snake_p, board);
    
    // Render final GAME OVER PRESS ANY KEY TO EXIT screen
    render_game_over(width, height);
    usleep(1000 * 1000);  // 1000ms
    cbreak(); // Leave halfdelay mode
    getch();
    

    // tell ncurses that we're done
    clear();
    endwin();
}

void end_game(size_t width, size_t height) {
    clear();
    render_game_end(width, height);
    usleep(1000 * 1000);  // 1000ms
    cbreak(); // Leave halfdelay mode
    getch();
    // tell ncurses that we're done
    endwin();
}

int main(int argc, char** argv) {
    // Main program function — this is what gets called when you run the
    // generated executable file from the command line!

    // Board data
    size_t width;   // the width of the board.
    size_t height;  // the height of the board.
    int* cells;     // a pointer to the first integer in an array of integers
                    // representing each board cell.

    // snake data (only used in part 3!)
    snake_t snake;    // your snake struct. (not used until part 3!)
    int snake_grows;  // 1 if snake should grow, 0 otherwise.

    enum board_init_status status;

    printf(
        "             ____   \n"
        "Hello       / . .\\ \n"
        "welcome to  \\  ---<\n"
        "snake        \\  /  \n"
        "   __________/ /    \n"
        "-=:___________/\n");
    do {
        char name_buffer[1000];
        read_name(name_buffer);
        g_name = name_buffer;
        g_name_len = mbslen(name_buffer);
        char* board = NULL;
        char board_buffer[100];
        read_board(board_buffer);
        int board_number = atoi(board_buffer);
        switch (board_number) {
            case 1:
                read_txt("board/1.txt", &board);
                break;
            case 2:
                read_txt("board/2.txt", &board);
                break;
            case 3:
                read_txt("board/3.txt", &board);
                break;
            default:
                break;

        }
        // initialize board from command line arguments
        switch (argc) {
            case (2):
                snake_grows = atoi(argv[1]);
                if (snake_grows != 1 && snake_grows != 0) {
                    printf(
                        "snake_grows must be either 1 (grows) or 0 (does not "
                        "grow)\n");
                    return 0;
                }
                status = initialize_game(&cells, &width, &height, &snake, board);
                break;
            case (3):
                snake_grows = atoi(argv[1]);
                if (snake_grows != 1 && snake_grows != 0) {
                    printf(
                        "snake_grows must be either 1 (grows) or 0 (does not "
                        "grow)\n");
                    return 0;
                } else if (*argv[2] == '\0') {
                    status = initialize_game(&cells, &width, &height, &snake, board);
                    break;
                }
                status = initialize_game(&cells, &width, &height, &snake, argv[2]);
                break;
            case (1):
            default:
                printf("usage: snake <GROWS: 0|1> [BOARD STRING]\n");
                return 0;
        }

        // Check validity of the board before rendering!
        if (status != INIT_SUCCESS) {
            return EXIT_FAILURE;
        }

        initialize_window(width, height);
        enum input_key real_input = INPUT_RIGHT;
        int speed = 0;
        while (!g_game_over) {
            enum input_key input = get_input();
            usleep(130000);
            if (input == INPUT_SPEEDUP) {
                speed = 2;
            } else if (input == INPUT_SPEEDDOWN) {
                speed = -2;
            } 
            if (input == INPUT_PAUSE && real_input == INPUT_PAUSE) {
                real_input = snake.snake_direction;
            } else if (input != INPUT_PAUSE && real_input == INPUT_PAUSE) {
                real_input = INPUT_PAUSE;
            } else if (input != INPUT_NONE) {
                real_input = input;
            }
            if (real_input != INPUT_PAUSE) {
                if (speed == 0) {
                    update(cells, width, height, &snake, input, snake_grows);
                }
                while (speed > 0) {
                    speed--;
                    update(cells, width, height, &snake, input, snake_grows);
                }
                if (speed < 0) {
                    speed++;
                }
            }
            render_game(cells, width, height);
        }
        over_game(cells, width, height, &snake, board);
        char status_buffer[100];
        do {
            read_status(status_buffer);
        } while (strcmp(status_buffer, "yes") != 0 && strcmp(status_buffer, "no"));
        insert(&scores, g_score, g_name); 
        if (strcmp(status_buffer, "yes") != 0) {
            break;
        }
    } while (1);
    end_game(width, height);
    output_rank();
}
