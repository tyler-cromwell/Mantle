/**********************************************************************
  Copyright (C) 2015 Tyler Cromwell <tjc6185@gmail.com>

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

// Kernel header(s)
#include <amd64/asm.h>
#include <amd64/i8259.h>
#include <kernel/types.h>

// Keyboard ports
#define KEYBOARD_DATA   0x60
#define KEYBOARD_STATUS 0x64

// US QWERTY keyboard map
static byte_t keymap[128] = {
    0,          /* <NOTHING> */
    27,         /* Escape */
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    '\n',       /* Enter / Keypad - Enter */
    0,          /* L-CTRL */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,          /* L-SHIFT */
    '\\',       /* Backslash */
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0,          /* R-SHIFT */
    '*',        /* Keypad - * */
    0,          /* L-ALT */
    ' ',        /* Space bar */
    0,          /* Caps lock */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /* F1 - F10 */
    0,          /* Num lock */
    0,          /* Scroll Lock */
    0,          /* Keypad - 7 / Home */
    0,          /* Keypad - 8 / Up */
    0,          /* Keypad - 9 / Page Up */
    '-',        /* Keypad - Dash (-) */
    0,          /* Keypad - 4 / Left */
    0,          /* Keypad - 5 */
    0,          /* Keypad - 6 / Right */
    '+',        /* Keypad - Plus (+) */
    0,          /* Keypad - 1 / End */
    0,          /* Keypad - 2 / Down */
    0,          /* Keypad - 3 / Page Down */
    0,          /* Keypad - 0 / Insert */
    0,          /* Keypad - Period (.) / Delete */
    0, 0, 0,    /* <NOTHING> */
    0, 0,       /* F11 - F12 Key */
    0,          /* All other keys are undefined */
};

// Newest character read from the keyboard
static volatile char next = -1;


/*
 * Basic PS/2 keyboard IRQ handler.
 *
 * Only called in "idt.c"
 */
void keyboard_handler(void) {
    byte_t status = inb(KEYBOARD_STATUS);

    if (status & 0x01) {
        char scan = inb(KEYBOARD_DATA);

        if (scan < 0) {
            return;
        }

        char key = keymap[scan];

        // Only printable characters
        if ((scan >= 0x02 && scan <= 0x0e) ||
            (scan >= 0x10 && scan <= 0x1c) ||
            (scan >= 0x1e && scan <= 0x29) ||
            (scan >= 0x2b && scan <= 0x35) ||
             scan == 0x39) {
            next = key;
        }
    }
}


/*
 * Basic getchar function.
 * Returns:
 *   The newest character typed.
 */
char keyboard_getchar(void) {
    i8259_clear_mask(I8259_IRQ_KEYBOARD);
    while (next == -1) {}                   // Wait for IRQ
    i8259_set_mask(I8259_IRQ_KEYBOARD);
    char c = next;
    next = -1;                              // Reset buffer
    return c;
}
