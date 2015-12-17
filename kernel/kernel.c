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
#include <drivers/console.h>
#include <kernel/string.h>
#include <kernel/version.h>

/* Linker Script Symbols */
extern struct undefined KERNEL_LMA;
extern struct undefined KERNEL_VMA;
extern struct undefined KERNEL_SIZE;

extern char* shell_readline(char *prompt);

/*
 * Dump information about the Kernel binary.
 */
void kinfo(void) {
    /* Get Kernel size */
    uint64_t size = ((uint64_t) &KERNEL_SIZE) / 1024;
    console_printf(FG_WHITE, "Kernel size: %uKB\n", size);
}

/*
 * Dump information about the Processor.
 */
void cpuinfo(void) {
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
 */
void multiboot(uint64_t magic, struct MultibootInfo *mbinfo) {
    /* Was the kernel booted by a Multiboot bootloader? */
    if (magic == MULTIBOOT_BOOT_MAGIC) {
        multiboot_init(mbinfo);
        multiboot_dump();
    }
}

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
    console_printf(FG_BLUE, STRING"\n");

    /* Setup interrupt handling */
    idt_configure();

    /* Wait for commands */
    while (1) {
        char *input = shell_readline("> ");

        /* Interpret input */
        if (!strncmp(input, "kinfo", strlen(input))) {
            kinfo();
        }
        else if (!strncmp(input, "cpuinfo", strlen(input))) {
            cpuinfo();
        }
        else if (!strncmp(input, "multiboot", strlen(input))) {
            multiboot(magic, mbinfo);
        }
        else if (!strncmp(input, "clear", strlen(input))) {
            console_clear();
        }
        else {
            console_printf(FG_WHITE, "%s\n", input);
        }
    }

    while (1) {}
    console_printf(BG_RED, "System halted");
    return;
}
