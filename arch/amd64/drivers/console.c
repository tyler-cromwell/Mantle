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
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

/* Kernel Headers */
#include <amd64/asm.h>
#include <lib/string.h>

/* Macro constants */
#define CONSOLE_START   (char*) 0xb8000 /* The starting address for video memory */
#define CONSOLE_END     (char*) 0xb8fa0 /* The ending address for video memory */
#define CHAR_WIDTH  2       /* The number of bytes used per character */
#define LINES       25      /* The number of lines on the screen */
#define LINE_CHARS  80      /* The number of characters per line */
#define LINE_BYTES  160     /* The number of bytes per line */
#define BYTES       4000    /* The number of usable bytes in video memory */

/* Pointer to the next "empty" character byte */
static char *next = CONSOLE_START;

/*
 * Moves cursor to the location of the
 * next available character.
 * Argument:
 *   uint8_t attribute: The color attribute.
 */
static void move_cursor(uint8_t attribute) {
    uint16_t pos = (next - CONSOLE_START) / 2;

    outb(0x3d4, 0x0f);
    outb(0x3d5, (uint8_t) ((pos >> 0) & 0xff));

    outb(0x3d4, 0x0e);
    outb(0x3d5, (uint8_t) ((pos >> 8) & 0xff));

    *(next+1) = *(next+1) | (attribute & 0x0f);
}

/*
 * Clears the console by zero-ing the screen buffer.
 * Side Effect:
 *   Resets the video pointer.
 */
void console_clear(void) {
    next = CONSOLE_START;
    memset(CONSOLE_START, 0, BYTES);
    move_cursor(0x00);
}

/*
 * Sets the background to a specific color.
 * Argument:
 *   uint8_t attribute: The color attribute.
 */
void console_set_background(uint8_t attribute) {
    next = CONSOLE_START;
    uint16_t offset = 0;

    while (offset < BYTES) {
        if (attribute >= 0x10) {
            next[offset+1] = (next[offset+1] % 0x10) + attribute;
        }
        offset += CHAR_WIDTH;
    }

    move_cursor(attribute);
}

/*
 * Writes a string of characters to the console.
 * Interprets the newline, backspace, and carriage return.
 * Arguments:
 *   char *message:  The message to write.
 *   uint16_t length: The number of bytes to write.
 *   uint8_t attribute: The coloring attribute.
 * Returns:
 *   The number of characters written.
 */
size_t console_write(char *message, size_t length, uint8_t attribute) {
    size_t c = 0;

    /* Ensure that the number of characters to write does not exceed the maximum */
    while (message[c] != '\0' && c < length) {
        if (next >= CONSOLE_END) {
            /* Shift the next pointer up one row */
            next -= LINE_BYTES;
            char *start = CONSOLE_START;

            /* Scroll everything up one row */
            for (uint16_t i = LINE_BYTES; i < BYTES; i++) {
                uint16_t j = i - LINE_BYTES;
                start[j] = start[i];
            }

            /* Clear the bottom line and preserve color */
            for (uint16_t i = 0; i < LINE_BYTES; i += CHAR_WIDTH) {
                next[i] = '\0';
            }
        }

        /* Interpret Newline */
        if (message[c] == '\n') {
            uint16_t remaining = LINE_CHARS - (((next - CONSOLE_START) % LINE_BYTES) / CHAR_WIDTH);

            for (uint8_t i = 0; i < remaining; i++) {
                *next = '\0';
                next += CHAR_WIDTH;
            }
        }
        /* Interpret Backspace */
        else if (message[c] == '\b') {
            char *second = CONSOLE_START + CHAR_WIDTH;

            if (next >= second) {
                 next -= CHAR_WIDTH;
                 *next = '\0';
            }
        }
        /* Interpret Carriage Return */
        else if (message[c] == '\r') {
            uint16_t line = next - CONSOLE_START;

            for (uint8_t r = line % LINE_BYTES; r > 0; r -= CHAR_WIDTH) {
                next -= CHAR_WIDTH;
                *next = '\0';
            }
        }
        /* Write everything else */
        else {
            *next = message[c];
            *(next+1) = (*(next+1) & 0xf0) | attribute;
            next += CHAR_WIDTH;
        }
        c++;
    }

    move_cursor(attribute);
    return c;
}

/*
 * Printf style function that writes a string of characters to the console.
 * Arguments:
 *   uint8_t attribute: The coloring attribute.
 *   char *format: The format string.
 *   ... : A variable length list of other arguments.
 * Returns:
 *   The number of characters written.
 */
size_t console_printf(uint8_t attribute, char *format, ...) {
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    size_t c = 0;

    /* Process each character */
    for (; *format != '\0'; format++) {
        if (*format != '%') {
            c += console_write(format, 1, attribute);
        } else {
            /* Increment again for tag character */
            char *tag = format+1;
            char *s = NULL;
            char b = 0;

            switch (*tag) {
                case 'c':
                    /* Character */
                    b = __builtin_va_arg(arguments, int64_t);
                    c += console_write(&b, 1, attribute);
                    format++;
                    break;
                case 'd':
                case 'i':
                    /* Signed 32-bit Integer */
                    s = itoa(__builtin_va_arg(arguments, int64_t), 10, 0);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case 'o':
                    /* Unsigned Octal Integer */
                    s = itoa(__builtin_va_arg(arguments, uint64_t), 8, 0);
                    c += console_write("0o", 2, attribute);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case 'p':
                    /* Pointer address */
                    s = itoa(__builtin_va_arg(arguments, uint64_t), 16, 0);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case 's':
                    /* String */
                    s = __builtin_va_arg(arguments, char*);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case 'u':
                    /* Unsigned 32-bit Integer */
                    s = itoa(__builtin_va_arg(arguments, uint64_t), 10, 0);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case 'x':
                    /* Unsigned Hexadecimal Integer (lowercase) */
                    s = itoa(__builtin_va_arg(arguments, uint64_t), 16, 0);
                    c += console_write("0x", 2, attribute);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case 'X':
                    /* Unsigned Hexadecimal Integer (uppercase) */
                    s = itoa(__builtin_va_arg(arguments, uint64_t), 16, 0);
                    strupper(s);
                    c += console_write("0x", 2, attribute);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case '%':
                    /* Percent sign */
                    b = '%';
                    c += console_write(&b, 1, attribute);
                    format++;
                    break;
                default:
                    c += console_write(format, 1, attribute);
            }
        }
    }

    __builtin_va_end(arguments);
    return c;
}
