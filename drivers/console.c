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
#include <kernel/string.h>

#define VGA_START   (char*) 0xb8000 /* The starting address for video memory */
#define VGA_END     (char*) 0xb8fa0 /* The ending address for video memory */

#define CHAR_WIDTH  2       /* The number of bytes used per character */
#define LINES       25      /* The number of lines on the screen */
#define LINE_CHARS  80      /* The number of characters per line */
#define LINE_BYTES  160     /* The number of bytes per line */
#define BYTES       4000    /* The number of usable bytes in video memory */

/* Pointer to the next "empty" character byte */
static char *next = VGA_START;

/*
 * Clears the console by zero-ing the screen buffer.
 * Side Effect:
 *   Resets the video pointer.
 */
void console_clear(void) {
    next = VGA_START;
    memset(VGA_START, 0, BYTES);
}

/*
 * Sets the background to a specific color.
 * Argument:
 *   uint8_t attribute: The color attribute.
 */
void console_set_background(uint8_t attribute) {
    next = VGA_START;
    uint16_t offset = 0;

    while (offset < BYTES) {
        if (attribute >= 0x10) {
            next[offset+1] = (next[offset+1] % 0x10) + attribute;
        }
        offset += CHAR_WIDTH;
    }
}

/*
 * Writes a string of characters to the console.
 * Will interpret the newline character.
 * Arguments:
 *   char *message:  The message to write.
 *   uint16_t length: The number of bytes to write.
 *   uint8_t attribute: The coloring attribute.
 * Returns:
 *   The number of characters written.
 * Side Effect:
 *   Updates the video pointer.
 */
size_t console_write(char *message, size_t length, uint8_t attribute) {
    size_t c = 0;

    /* Ensure that the number of characters to write does not exceed the maximum */
    while (message[c] != '\0' && c < length) {
        if (next >= VGA_END) {
            /* Shift the next pointer up one row */
            next -= LINE_BYTES;
            char *start = VGA_START;

            /* Scroll everything up one row */
            for (uint16_t i = LINE_BYTES; i < BYTES; i++) {
                uint16_t j = i - LINE_BYTES;
                start[j] = start[i];
            }

            /* Clear the bottom line */
            memset(next, 0, LINE_BYTES);
        }

        /* Interpret the newline character */
        if (message[c] == '\n') {
            uint16_t remaining = LINE_CHARS - (((next - VGA_START) % LINE_BYTES) / CHAR_WIDTH);

            /* Write blank characters for rest of the line */
            for (uint8_t i = 0; i < remaining; i++) {
                *next = '\0';
                next += CHAR_WIDTH;
            }
        }
        else {
            *next = message[c];
            *(next+1) = *(next+1) | attribute;
            next += CHAR_WIDTH;
        }
        c++;
    }

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
    size_t c = 0;
    va_list arguments;
    va_start(arguments, format);

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
                    b = va_arg(arguments, int32_t);
                    c += console_write(&b, 1, attribute);
                    format++;
                    break;
                case 'd':
                    /* Signed 32-bit Integer */
                    s = itoa(va_arg(arguments, int32_t), 10);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case 'o':
                    /* Unsigned Octal Integer */
                    s = itoa(va_arg(arguments, uint32_t), 8);
                    c += console_write("0o", 2, attribute);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case 's':
                    /* String */
                    s = va_arg(arguments, char*);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case 'u':
                    /* Unsigned 32-bit Integer */
                    s = itoa(va_arg(arguments, uint32_t), 10);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case 'x':
                    /* Unsigned Hexadecimal Integer (lowercase) */
                    s = itoa(va_arg(arguments, uint32_t), 16);
                    c += console_write("0x", 2, attribute);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                case 'X':
                    /* Unsigned Hexadecimal Integer (uppercase) */
                    s = itoa(va_arg(arguments, uint32_t), 16);
                    strupper(s);
                    c += console_write("0x", 2, attribute);
                    c += console_write(s, strlen(s), attribute);
                    format++;
                    break;
                default:
                    c += console_write(format, 1, attribute);
            }
        }
    }

    va_end(arguments);
    return c;
}
