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

/* External - Defined in "keyboard.c" */
extern char keyboard_key;

static char command[16] = {0};
static char *next = command;
static char *end = command + 15;

/*
 * Basic text input function.
 * Argument:
 *   char *prompt: the prompt to display.
 * Returns:
 *   a pointer to the command buffer.
 */
char* shell_readline(char *prompt) {
    console_printf(FG_WHITE, "%s", prompt);
    i8259_clear_mask(I8259_IRQ_KEYBOARD);   /* Enable keyboard */

    while (1) {
        /* Handle Newline */
        if (keyboard_key == '\n') {
            console_printf(FG_WHITE, "%c", keyboard_key);
            keyboard_key = -1;
            break;
        }
        /* Handle Backspace */
        else if (keyboard_key == '\b' && next != command) {
            next--;
            *next = '\0';
            console_printf(FG_WHITE, "%c", keyboard_key);
            keyboard_key = -1;
        }
        /* Handle printable characters */
        else if (keyboard_key >= 32 && keyboard_key <= 126 && next != end) {
            *next = keyboard_key;
            *next++;
            console_printf(FG_WHITE, "%c", keyboard_key);
            keyboard_key = -1;
        }
    }

    i8259_set_mask(I8259_IRQ_KEYBOARD); /* Disable keyboard */
    return command;
}
