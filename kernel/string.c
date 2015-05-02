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
    if (number > 0) {
        while (number != 0) {
            *--string = 48 + (number % 10);
            number /= 10;
        }
    } else if (number < 0) {
        while (number != 0) {
            *--string = 48 - (number % 10);
            number /= 10;
        }
        *--string = '-';
    } else {
        buffer[0] = 48;
        string = buffer;
    }
    return string;
}

/*
 * Copies a string.
 * Arguments:
 *   char* dest: The new string.
 *   const char* src: The source string.
 *   size_t n: The number of bytes to copy.
 * Returns:
 *   A pointer to the new string.
 */
char* strncpy(char* dest, const char* src, size_t n) {
    for (size_t i = 0; i < n || src[i] == '\0'; i++)
        dest[i] = src[i];
    return dest;
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
