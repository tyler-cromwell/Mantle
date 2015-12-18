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

/* Kernel Headers */
#include <amd64/i8259.h>
#include <drivers/console.h>
#include <kernel/string.h>

/* External - functions / variables */
extern char keyboard_getchar(void); /* Defined in "keyboard.c" */

/* Internal variables */
static char input[16] = {0};

/*
 * Basic text input function.
 * Argument:
 *   char *prompt: the prompt to display.
 * Returns:
 *   a pointer to the input buffer.
 */
char* shell_readline(char *prompt) {
    char *next = input;
    char *end = input + 15;
    uint8_t backspaces = 0;

    console_printf(FG_WHITE, "%s", prompt);
    memset(input, '\0', 16);
    i8259_clear_mask(I8259_IRQ_KEYBOARD);   /* Enable keyboard */

    while (1) {
        char letter = keyboard_getchar();

        /* Handle Newline */
        if (letter == '\n') {
            backspaces = 0;
            console_printf(FG_WHITE, "%c", letter);
            break;
        }
        /* Handle Backspace */
        else if (letter == '\b' && next != input) {
            backspaces--;
            next--;
            *next = '\0';
            console_printf(FG_WHITE, "%c", letter);
        }
        /* Handle printable characters */
        else if (letter >= 32 && letter <= 126 && next != end) {
            backspaces++;
            *next = letter;
            *next++;
            console_printf(FG_WHITE, "%c", letter);
        }
    }

    i8259_set_mask(I8259_IRQ_KEYBOARD); /* Disable keyboard */
    return input;
}
