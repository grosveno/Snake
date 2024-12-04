#include "common.h"

#include <stdlib.h>
#include "priority_queue.h"

// Definition of global variables for game status.
int g_game_over;
int g_score;
int g_name_len;
char* g_name;
PriorityQueue* scores;
/** Sets the seed for random number generation.
 * Arguments:
 *  - `seed`: the seed.
 */
void set_seed(unsigned seed) {
    /* DO NOT MODIFY THIS FUNCTION */
    srand(seed);
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Returns a random index in [0, size)
 * Arguments:
 *  - `size`: the upper bound for the generated value (exclusive).
 */
unsigned generate_index(unsigned size) {
    return rand() % size;
}
