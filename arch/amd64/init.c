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

/* Kernel header(s) */
#include <amd64/console.h>
#include <amd64/multiboot.h>
#include <amd64/shell.h>
#include <kernel/types.h>
#include <kernel/version.h>
#include <lib/string.h>

/* External functions / variables */
extern void idt_configure(void);                            /* Defined in "idt.c" */
extern void paging_configure(size_t memory, ulong_t pages); /* Defined in "paging.c" */
extern void paging_pageinfo(void);                          /* Defined in "paging.c" */


/*
 * Early kernel setup, initializes critical components.
 * The system will halt when/if this function returns.
 * Arguments:
 *   ulong_t magic: A Multiboot bootloaders magic number.
 *   struct MultibootInfo *mbinfo:
 *       The physical memory address of the Multiboot information struct.
 *   ulong_t pages: Amount of Pages in use at boot.
 */
void init_kernel(ulong_t magic, struct MultibootInfo *mbinfo, ulong_t pages) {
    console_clear();
    console_printf(FG_BLUE_L, STRING"\n");

    /* Setup critical components */
    multiboot_init(mbinfo);
    idt_configure();            /* Interrupts */
    paging_configure(           /* Virtual Memory */
        multiboot_memsize(),
        pages
    );

    console_printf(FG_WHITE, "\n");

    /* Wait for commands */
    while (1) {
        char *input = shell_readline("> ");

        /* Interpret input */
        if (strlcmp(input, "kinfo") > 0) {
            shell_cmd_kinfo();
        }
        else if (strlcmp(input, "cpuinfo") > 0) {
            shell_cmd_cpuinfo();
        }
        else if (strlcmp(input, "multiboot") > 0) {
            shell_cmd_multiboot(magic);
        }
        else if (strlcmp(input, "pageinfo") > 0) {
            paging_pageinfo();
        }
        else if (strlcmp(input, "clear") > 0) {
            console_clear();
        }
        else if (strlcmp(input, "help") > 0) {
            console_printf(FG_WHITE, "clear: Clears the screen.\n");
            console_printf(FG_WHITE, "cpuinfo: Prints CPUID information.\n");
            console_printf(FG_WHITE, "halt: Exits the shell and halts.\n");
            console_printf(FG_WHITE, "kinfo: Prints information about the kernel.\n");
            console_printf(FG_WHITE, "pageinfo: Prints information about the paging tree.\n");
            console_printf(FG_WHITE, "multiboot: Prints Multiboot information.\n");
        }
        else if (strlcmp(input, "halt") > 0) {
            break;
        }
        else if (strlen(input) > 0) {
            console_printf(FG_WHITE, "Unkown command \"%s\"\n", input);
        }

        /* Reset input buffer */
        memset(input, '\0', SHELL_BUFSIZ);
    }

    console_printf(FG_WHITE | BG_RED, "System halted");
    return;
}
