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
#include <amd64/amd64.h>
#include <amd64/multiboot.h>
#include <amd64/shell.h>
#include <drivers/console.h>
#include <kernel/string.h>
#include <kernel/version.h>

/* External */
void idt_configure(void);   /* Defined in "idt.c" */

/*
 * The main kernel function; this is where Ritchie begins operation.
 * The system will halt when/if this function returns.
 * Arguments:
 *   uint64_t magic: A Multiboot bootloaders magic number.
 *   struct MultibootInfo *mbinfo:
 *       The physical memory address of the Multiboot information struct.
 */
void kernel_main(uint64_t magic, struct MultibootInfo *mbinfo) {
    console_clear();
    console_printf(FG_BLUE_L, STRING"\n");

    /* Setup interrupt handling */
    idt_configure();

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
            shell_cmd_multiboot(magic, mbinfo);
        }
        else if (strlcmp(input, "clear") > 0) {
            console_clear();
        }
        else if (strlen(input) > 0) {
            console_printf(FG_WHITE, "Unkown command \"%s\"\n", input);
        }

        /* Reset input buffer */
        memset(input, '\0', SHELL_BUFSIZ);
    }

    while (1) {}
    console_printf(BG_RED, "System halted");
    return;
}
