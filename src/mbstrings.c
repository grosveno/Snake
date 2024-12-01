#include "mbstrings.h"

#include <stddef.h>

/* mbslen - multi-byte string length
 * - Description: returns the number of UTF-8 code points ("characters")
 * in a multibyte string. If the argument is NULL or an invalid UTF-8
 * string is passed, returns -1.
 *
 * - Arguments: A pointer to a character array (`bytes`), consisting of UTF-8
 * variable-length encoded multibyte code points.
 *
 * - Return: returns the actual number of UTF-8 code points in `src`. If an
 * invalid sequence of bytes is encountered, return -1.
 *
 * - Hints:
 * UTF-8 characters are encoded in 1 to 4 bytes. The number of leading 1s in the
 * highest order byte indicates the length (in bytes) of the character. For
 * example, a character with the encoding 1111.... is 4 bytes long, a character
 * with the encoding 1110.... is 3 bytes long, and a character with the encoding
 * 1100.... is 2 bytes long. Single-byte UTF-8 characters were designed to be
 * compatible with ASCII. As such, the first bit of a 1-byte UTF-8 character is
 * 0.......
 *
 * You will need bitwise operations for this part of the assignment!
 */
size_t mbslen(const char* bytes) {
    // Check if the input string is NULL
    if (bytes == NULL) {
        return -1;
    }
    
    size_t len = 0; // Initialize the character count to 0.
    
    // Loop through the string until we reach the null terminator.
    while (*bytes) {
        unsigned char current_byte = *bytes; // Get the current byte.
        
        // Check if the current byte is the start of a UTF-8 character.
        if ((current_byte & 0x80) == 0) {
            // 1-byte character (ASCII compatible).
            bytes += 1;
        } else if ((current_byte & 0xE0) == 0xC0) {
            // 2-byte character.
            bytes += 2;
        } else if ((current_byte & 0xF0) == 0xE0) {
            // 3-byte character.
            bytes += 3;
        } else if ((current_byte & 0xF8) == 0xF0) {
            // 4-byte character.
            bytes += 4;
        } else {
            // Invalid UTF-8 encoding.
            return -1; // Return 0 for error.
        }
        
        len += 1; // Increment the character count for each valid character start byte.
    }
    
    return len; // Return the total character count.
}