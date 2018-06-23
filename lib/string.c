/**********************************************************************
  Copyright (C) 2015-Present Tyler Cromwell <tjc6185@gmail.com>

  This file is part of Mantle.

  Mantle is free software: you can redistribute it and/or modify
  it under Version 2 of the terms of the GNU General Public License
  as published by the Free Software Foundation.

  Mantle is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Mantle.
  If not, see <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>
**********************************************************************/

// C Standard Library header(s)
#include <stddef.h>

// Kernel header(s)
#include <kernel/types.h>
#include <lib/string.h>

#define ITOA_BUFSIZ 65


char* itoa(struct ItoaOptions *opts, long number) {
    static char buffer[ITOA_BUFSIZ] = {0};
    char *string = buffer + ITOA_BUFSIZ - 1;
    ushort_t c = 0;
    uchar_t base = 0;

    // Determine base
    if (opts->binary) base = 2;
    else if (opts->octal) base = 8;
    else if (opts->hex) base = 16;
    else base = 10;

    memset(buffer, 0, ITOA_BUFSIZ);

    // If number is zero, just stop
    if (number == 0) {
        *--string = '0';
        c++;
    }
    // Convert, up to a base of 16
    else {
        long i = number;

        /* Do convertion */
        while (i != 0) {
            *--string = "fedcba9876543210123456789abcdef"[15 + i % base];
            i /= base;
            c++;
        }

        /* If negative */
        if (number < 0 && base == 10) {
            *--string = '-';
        }
    }

    // Print leading zeros
    if (opts->pad) {
        uchar_t d = 0;
        if (base == 2) d = 64;
        else if (base == 8) d = 22;
        else if (base == 16) d = 16;

        for (; c < d && string != buffer; c++) {
            *--string = '0';
        }
    }

    return string;
}


void *memcpy(void *dest, const void *src, size_t n) {
    byte_t *buf = dest;
    for (size_t i = 0; i < n; i++) {
        buf[i] = ((byte_t*) src)[i];
    }
    return dest;
}


void *memset(void *dest, int c, size_t n) {
    byte_t *buf = dest;
    for (size_t i = 0; i < n; i++) {
        buf[i] = (byte_t) c;
    }
    return dest;
}


int8_t strlcmp(const char *s1, const char *s2) {
    // If a string is NULL, return an error
    if (s1 == NULL) {
        return -1;
    }
    else if (s2 == NULL) {
        return -2;
    }

    // Get lengths
    size_t sl1 = strlen(s1);
    size_t sl2 = strlen(s2);
    size_t l = 0;

    // Compare length before content
    if (sl1 == sl2) {
        return !strncmp(s1, s2, sl1);
    } else {
        return 0;
    }
}


int strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if ((s1[i] - s2[i]) != 0) {
            return s1[i] - s2[i];
        }
    }
    return 0;
}


char* strncpy(char *dest, const char *src, size_t n) {
    for (size_t i = 0; i < n || src[i] == '\0'; i++) {
        dest[i] = src[i];
    }
    return dest;
}


size_t strlen(const char *string) {
    size_t length = 0;
    for (; *string++ != '\0'; length++);
    return length;
}


size_t strnlen(const char *string, size_t n) {
    size_t length = 0;
    for (size_t i = 0; string[i] != '\0' && i < n; i++) {
        length++;
    }
    return length;
}


void strlower(char *string) {
    char *s = string;
    size_t n = strlen(string);

    // Iterate until NULL byte or string length
    for (size_t i = 0; i < n && *s != '\0'; i++, s++) {
        if (*s >= 'A' && *s <= 'Z') {
            *s = *s + 32;
        }
    }
}


void strupper(char *string) {
    char *s = string;
    size_t n = strlen(string);

    // Iterate until NULL byte or string length
    for (size_t i = 0; i < n && *s != '\0'; i++, s++) {
        if (*s >= 'a' && *s <= 'z') {
            *s = *s - 32;
        }
    }
}
