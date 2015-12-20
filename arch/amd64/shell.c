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
#include <amd64/console.h>
#include <amd64/cpuid.h>
#include <amd64/i8259.h>
#include <amd64/shell.h>
#include <amd64/multiboot.h>
#include <lib/string.h>

/* Linker Script Symbols */
extern struct undefined KERNEL_LMA;
extern struct undefined KERNEL_SIZE;

/* External functions / variables */
extern char keyboard_getchar(void); /* Defined in "keyboard.c" */

/* Internal variables */
static char input[SHELL_BUFSIZ] = {0};

/*
 * Basic text input function.
 * Argument:
 *   char *prompt: The prompt to display.
 * Returns:
 *   a pointer to the input buffer.
 */
char* shell_readline(char *prompt) {
    char *next = input;
    char *end = input + SHELL_BUFSIZ - 1;
    uint8_t backspaces = 0;

    console_printf(FG_WHITE, "%s", prompt);
    memset(input, '\0', SHELL_BUFSIZ);
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

/*
 * Dump information about the Kernel binary.
 */
void shell_cmd_kinfo(void) {
    /* Get Kernel size */
    uint64_t size = ((uint64_t) &KERNEL_SIZE) / 1024;
    console_printf(FG_WHITE, "Size in memory: %uKB\n", size);
    console_printf(FG_WHITE, "Physical address: %x\n", &KERNEL_LMA);
}

/*
 * Dump information about the Processor.
 */
void shell_cmd_cpuinfo(void) {
    /* Get CPU vendor name */
    char id[13] = {0};
    cpuid_vendor(id);
    console_printf(FG_WHITE, "vendor_id: ");

    /* Print CPU vendor name */
    if (!strncmp(id, VENDOR_INTEL, strlen(id))) {
        console_printf(FG_CYAN_L, "%s\n", id);
    } else if (!strncmp(id, VENDOR_AMD, strlen(id))) {
        console_printf(FG_RED_L, "%s\n", id);
    } else {
        console_printf(FG_GREY_L, "%s\n", id);
    }

    /* Get and print number of processors */
    console_printf(FG_WHITE, "processors: %u\n", cpuid_cpus());
}

/*
 * Dump Multiboot information.
 * Arguments:
 *   uint64_t magic: Bootloader magic number.
 *   struct MultibootInfo *mbinfo: Pointer to the Multiboot information.
 */
void shell_cmd_multiboot(uint64_t magic, struct MultibootInfo *mbinfo) {
    /* Was the kernel booted by a Multiboot bootloader? */
    if (magic == MULTIBOOT_BOOT_MAGIC) {
        multiboot_init(mbinfo);
        multiboot_dump();
    }
}
