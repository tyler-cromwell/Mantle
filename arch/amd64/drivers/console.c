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
#define TAB_WIDTH   4       /* The number of characters to advance by for indentation */
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
 *   uint8_t color: The color.
 */
static void move_cursor(uint8_t color) {
    uint16_t pos = (next - CONSOLE_START) / 2;

    outb(0x3d4, 0x0f);
    outb(0x3d5, (uint8_t) ((pos >> 0) & 0xff));

    outb(0x3d4, 0x0e);
    outb(0x3d5, (uint8_t) ((pos >> 8) & 0xff));

    *(next+1) = *(next+1) | (color & 0x0f);
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
 *   uint8_t color: The color.
 */
void console_set_background(uint8_t color) {
    next = CONSOLE_START;
    uint16_t offset = 0;

    while (offset < BYTES) {
        if (color >= 0x10) {
            next[offset+1] = (next[offset+1] % 0x10) + color;
        }
        offset += CHAR_WIDTH;
    }

    move_cursor(color);
}

/*
 * Writes a string of characters to the console.
 * Interprets the newline, backspace, and carriage return.
 * Arguments:
 *   uint8_t color: The color.
 *   char *message:  The message to write.
 *   uint16_t length: The number of bytes to write.
 * Returns:
 *   The number of characters written.
 */
size_t console_write(uint8_t color, char *message, size_t length) {
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
        /* Interpret Tab */
        else if (message[c] == '\t') {
            uint8_t indent = TAB_WIDTH;
            uint64_t n = (uint64_t) next;

            /* Determine amount to indent by */
            while ((n + (indent * CHAR_WIDTH)) % (TAB_WIDTH * CHAR_WIDTH) != 0) {
                indent--;
            }

            /* Indent */
            for (uint8_t i = 0; i < indent; i++) {
                *next = ' ';
                *(next+1) = (*(next+1) & 0xf0) | color;
                next += CHAR_WIDTH;
            }
        }
        /* Write everything else */
        else {
            *next = message[c];
            *(next+1) = (*(next+1) & 0xf0) | color;
            next += CHAR_WIDTH;
        }
        c++;
    }

    move_cursor(color);
    return c;
}

/*
 * Printf style function that writes a string of characters to the console.
 * Arguments:
 *   uint8_t color: The coloring color.
 *   char *format: The format string.
 *   ... : A variable length list of other arguments.
 * Returns:
 *   The number of characters written.
 */
size_t console_printf(uint8_t color, char *format, ...) {
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    size_t c = 0;

    /* Process each character */
    for (; *format != '\0'; format++) {
        if (*format != '%') {
            c += console_write(color, format, 1);
        }
        else {
            struct ItoaOptions opts = {0};
            memset(&opts, 0, sizeof(struct ItoaOptions));

            /* Increment again for tag character */
            char *tag = format+1;
            char *s = NULL;
            char ch = 0;

            switch (*tag) {
                case 'B':
                    /* Binary Integer (leading zeros) */
                    opts.pad = 1;
                case 'b':
                    /* Binary Integer (no leading zeros) */
                    opts.binary = 1;
                    s = itoa(&opts, __builtin_va_arg(arguments, uint64_t));
                    c += console_write(color, "0b", 2);
                    c += console_write(color, s, strlen(s));
                    format++;
                    break;
                case 'c':
                    /* Character */
                    ch = __builtin_va_arg(arguments, int32_t);
                    c += console_write(color, &ch, 1);
                    format++;
                    break;
                case 'd':
                case 'i':
                    /* Signed 64-bit Integer */
                    s = itoa(&opts, __builtin_va_arg(arguments, int64_t));
                    c += console_write(color, s, strlen(s));
                    format++;
                    break;
                case 'O':
                    /* Octal Integer (leading zeros, lowercase) */
                    opts.pad = 1;
                case 'o':
                    /* Octal Integer (no leading zeros, lowercase) */
                    opts.octal = 1;
                    s = itoa(&opts, __builtin_va_arg(arguments, uint64_t));
                    c += console_write(color, "0o", 2);
                    c += console_write(color, s, strlen(s));
                    format++;
                    break;
                case 'p':
                    /* Pointer address */
                    opts.hex = 1;
                    s = itoa(&opts, __builtin_va_arg(arguments, uint64_t));
                    c += console_write(color, s, strlen(s));
                    format++;
                    break;
                case 's':
                    /* String */
                    s = __builtin_va_arg(arguments, char*);
                    c += console_write(color, s, strlen(s));
                    format++;
                    break;
                case 'u':
                    /* Unsigned 64-bit Integer */
                    s = itoa(&opts, __builtin_va_arg(arguments, uint64_t));
                    c += console_write(color, s, strlen(s));
                    format++;
                    break;
                case 'X':
                    /* Hexadecimal Integer (leading zeros, lowercase) */
                    opts.pad = 1;
                case 'x':
                    /* Hexadecimal Integer (no leading zeros, lowercase) */
                    opts.hex = 1;
                    s = itoa(&opts, __builtin_va_arg(arguments, uint64_t));
                    c += console_write(color, "0x", 2);
                    c += console_write(color, s, strlen(s));
                    format++;
                    break;
                case '%':
                    /* Percent sign */
                    ch = '%';
                    c += console_write(color, &ch, 1);
                    format++;
                    break;
                default:
                    c += console_write(color, format, 1);
            }
        }
    }

    __builtin_va_end(arguments);
    return c;
}
