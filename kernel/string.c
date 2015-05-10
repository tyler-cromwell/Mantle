/* C Standard Library Headers,
   these don't need to link against libc */
#include <stddef.h>
#include <stdint.h>

#include <kernel/string.h>

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
 * Checks if 2 strings are the same. Returns 0 if they are equal.
 * If they are not equal, returns the first character in s1 that is different.
 * Arguments:
 *   const char* s1: The first string.
 *   const char* s2: The second string.
 *   size_t n: The number of bytes to compare.
 * Returns (see above)
 */
int32_t strncmp(const char* s1, const char* s2, size_t n) {
    for (size_t i = 0; i < n && i < strlen(s1); i++) {
        if ((s1[i] - s2[i]) != 0)
            return s1[i] - s2[i];
    }
    return 0;
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

/*
 * Calculates the length of a string, up to a given size.
 * Argument:
 *   const char* string: The string to find the length of.
 *   size_t n: The counting limit
 * Returns:
 *   The length of string.
 */
size_t strnlen(const char* string, size_t n) {
    size_t length = 0;
    for (size_t i = 0; string[i] != '\0' && i < n; i++)
        length++;
    return length;
}
