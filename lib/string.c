/**********************************************************************
  Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>

  This file is part of Ritchie.

  Ritchie is free software: you can redistribute it and/or modify
  it under Version 2 of the terms of the GNU General Public License
  as published by the Free Software Foundation.

  Ritchie is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Ritchie.
  If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
**********************************************************************/

/* C Standard Library Headers,
   these don't need to link against libc */
#include <stddef.h>
#include <stdint.h>

/* Kernel Headers */
#include <lib/string.h>

#define ITOA_BUFSIZ 65

/*
 * Creates the string representation of a number.
 * Argument:
 *   struct ItoaOptions *opts:
 *     Pointer to the options that determine
 *     how the number should be converted.
 *   uint64_t number: The raw bits of the number.
 * Returns:
 *   The string representation of number.
 */
char* itoa(struct ItoaOptions *opts, uint64_t raw) {
    static char buffer[ITOA_BUFSIZ] = {0};
    char *string = buffer + ITOA_BUFSIZ - 1;
    uint16_t c = 0;
    uint8_t base = 0;

    /* Determine base */
    if (opts->binary) base = 2;
    else if (opts->octal) base = 8;
    else if (opts->hex) base = 16;
    else base = 10;

    memset(buffer, 0, ITOA_BUFSIZ);

    /* If number is zero, just stop */
    if (raw == 0) {
        *--string = '0';
        c++;
    }
    /* Convert, up to a base of 16 */
    else {
        /* Parse signed */
        if (opts->sign) {
            int64_t i = (int32_t) raw;

            while (i != 0) {
                *--string = "fedcba9876543210123456789abcdef"[15 + i % base];
                i /= base;
                c++;
            }

            if (!(opts->hex && opts->octal && opts->binary)) {
                *--string = '-';
            }
        }
        /* Parse unsigned */
        else {
            uint64_t u = raw;

            while (u != 0) {
                *--string = "0123456789abcdef"[u % base];
                u /= base;
                c++;
            }
        }
    }

    /* Print leading zeros */
    if (opts->pad) {
        uint8_t d = 0;
        if (base == 2) d = 64;
        else if (base == 8) d = 22;
        else if (base == 16) d = 16;

        for (; c < d && string != buffer; c++) {
            *--string = '0';
        }
    }

    return string;
}

/*
 * Copies the contents of one buffer to another.
 * Arguments:
 *   void *dest: The destination buffer.
 *   const void *src: The source buffer.
 *   size_t n: The number of bytes to copy.
 * Returns:
 *   A pointer to the destination buffer.
 */
void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *buf = dest;
    for (size_t i = 0; i < n; i++) {
        buf[i] = ((uint8_t*) src)[i];
    }
    return dest;
}

/*
 * Fills the first 'n' bytes of 'dest' with 'c'.
 * Arguments:
 *   void *dest: The region of memory to be over-written.
 *   int c: The value to write with.
 *   size_t n: The number of bytes to write.
 * Returns:
 *   A pointer to the destination buffer.
 */
void *memset(void *dest, int c, size_t n) {
    uint8_t *buf = dest;
    for (size_t i = 0; i < n; i++) {
        buf[i] = (uint8_t) c;
    }
    return dest;
}

/*
 * Compares the contents of 2 strings, taking length into account.
 * Mostly a wrapper for strncmp.
 * Arguments:
 *   const char *s1: The first string.
 *   const char *s2: The second string.
 * Returns:
 *   A status/error code:
 *     1 if equal, 0 if unequal
 *     -1 if s1 is NULL, -2 if s2 is
 */
int8_t strlcmp(const char *s1, const char *s2) {
    /* If a string is NULL, return an error */
    if (s1 == NULL) {
        return -1;
    }
    else if (s2 == NULL) {
        return -2;
    }

    /* Get lengths */
    size_t sl1 = strlen(s1);
    size_t sl2 = strlen(s2);
    size_t l = 0;

    /* Compare length before content */
    if (sl1 == sl2) {
        return !strncmp(s1, s2, sl1);
    } else {
        return 0;
    }
}

/*
 * Compares the contents of 2 strings. Returns 0 if they are equal.
 * If they are not equal, returns the first character in s1 that is different.
 * Arguments:
 *   const char *s1: The first string.
 *   const char *s2: The second string.
 *   size_t n: The number of bytes to compare.
 * Returns (see above)
 */
int32_t strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if ((s1[i] - s2[i]) != 0) {
            return s1[i] - s2[i];
        }
    }
    return 0;
}

/*
 * Copies a string.
 * Arguments:
 *   char *dest: The new string.
 *   const char *src: The source string.
 *   size_t n: The number of bytes to copy.
 * Returns:
 *   A pointer to the new string.
 */
char* strncpy(char *dest, const char *src, size_t n) {
    for (size_t i = 0; i < n || src[i] == '\0'; i++) {
        dest[i] = src[i];
    }
    return dest;
}


/*
 * Calculates the length of a string.
 * Argument:
 *   const char *string: The string to find the length of.
 * Returns:
 *   The length of string.
 */
size_t strlen(const char *string) {
    size_t length = 0;
    for (; *string++ != '\0'; length++);
    return length;
}

/*
 * Calculates the length of a string, up to a given size.
 * Argument:
 *   const char *string: The string to find the length of.
 *   size_t n: The counting limit
 * Returns:
 *   The length of string.
 */
size_t strnlen(const char *string, size_t n) {
    size_t length = 0;
    for (size_t i = 0; string[i] != '\0' && i < n; i++) {
        length++;
    }
    return length;
}

/*
 * Converts the alphabetical characters
 * in a string to lowercase.
 * Argument:
 *   char *string: The string to be modified.
 */
void strlower(char *string) {
    char *s = string;
    size_t n = strlen(string);

    /* Iterate until NULL byte or string length */
    for (size_t i = 0; i < n && *s != '\0'; i++, s++) {
        if (*s >= 'A' && *s <= 'Z') {
            *s = *s + 32;
        }
    }
}

/*
 * Converts the alphabetical characters
 * in a string to uppercase.
 * Argument:
 *   char *string: The string to be modified.
 */
void strupper(char *string) {
    char *s = string;
    size_t n = strlen(string);

    /* Iterate until NULL byte or string length */
    for (size_t i = 0; i < n && *s != '\0'; i++, s++) {
        if (*s >= 'a' && *s <= 'z') {
            *s = *s - 32;
        }
    }
}
