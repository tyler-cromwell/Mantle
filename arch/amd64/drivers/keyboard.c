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
#include <stdint.h>

/* Kernel Headers */
#include <amd64/amd64.h>
#include <drivers/console.h>

/* Keyboard ports */
#define KEYBOARD_CMD    0x64
#define KEYBOARD_DATA   0x60

/* US QWERTY keyboard map */
static uint8_t keyboard_map[128] = {
    0,      /* <NOTHING> */
    27,     /* Escape */
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    '\n',   /* Enter / Keypad - Enter */
    0,      /* L-CTRL */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,      /* L-SHIFT */
    '\\',   /* Backslash */
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0,      /* R-SHIFT */
    '*',    /* Keypad - * */
    0,      /* L-ALT */
    ' ',    /* Space bar */
    0,      /* Caps lock */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /* F1 - F10 */
    0,      /* Num lock */
    0,      /* Scroll Lock */
    0,      /* Keypad - 7 / Home */
    0,      /* Keypad - 8 / Up */
    0,      /* Keypad - 9 / Page Up */
    '-',    /* Keypad - Dash (-) */
    0,      /* Keypad - 4 / Left */
    0,      /* Keypad - 5 */
    0,      /* Keypad - 6 / Right */
    '+',    /* Keypad - Plus (+) */
    0,      /* Keypad - 1 / End */
    0,      /* Keypad - 2 / Down */
    0,      /* Keypad - 3 / Page Down */
    0,      /* Keypad - 0 / Insert */
    0,      /* Keypad - Period (.) / Delete */
    0, 0, 0,    /* <NOTHING> */
    0, 0,   /* F11 - F12 Key */
    0,      /* All other keys are undefined */
};

/* Amount of allowable backspaces */
static uint16_t backspaces = 0;

/* Most recent key pressed */
char keyboard_key = -1;

/*
 * Handler function for the Keyboard IRQ.
 */
void keyboard_handler(void) {
    uint8_t status = inb(KEYBOARD_CMD);

    if (status & 0x01) {
        char keycode = inb(KEYBOARD_DATA);
        if (keycode < 0) return;

        char letter = keyboard_map[keycode];

        /* Only printable characters */
        if ((keycode >= 0x02 && keycode <= 0x0d) ||
            (keycode >= 0x10 && keycode <= 0x1d) ||
            (keycode >= 0x1e && keycode <= 0x29) ||
            (keycode >= 0x2b && keycode <= 0x35) ||
             keycode == 0x39) {
            backspaces++;
        }
        /* Handle Enter */
        else if (keycode == 0x1c) {
            backspaces = 0;
        }
        /* Handle Backspace */
        else if (keycode == 0x0e && backspaces >= 1) {
            backspaces--;
        }

        keyboard_key = letter;
    }
}
