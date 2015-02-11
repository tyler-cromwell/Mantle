/* C Standard Library Headers,
   these don't need to link against libc */
#include <stddef.h>

/*
 * Creates the string representation of a number.
 * Argument:
 *   long number: The number to be represented.
 * Returns:
 *   The string representation of number.
 */
char* itoa(long number) {
    static char buffer[12];
    char* string = buffer + 11;
    if (number >= 0) {
        while (number != 0) {
            *--string = 48 + (number % 10);
            number /= 10;
        }
    } else {
        while (number != 0) {
            *--string = 48 - (number % 10);
            number /= 10;
        }
        *--string = '-';
    }
    return string;
}

/*
 * Calculates the length of a string.
 * Argument:
 *   const char* string: The string to find the length of.
 * Returns:
 *   The length of string.
 */
size_t strlen(const char* string) {
    size_t length = 0;
    for (; *string++ != '\0'; length++);
    return length;
}
