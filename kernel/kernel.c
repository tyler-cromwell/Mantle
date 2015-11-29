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
#include <drivers/console.h>
#include <kernel/string.h>
#include <kernel/version.h>
#include <x86/multiboot.h>
#include <x86/x86.h>

//#define VENDOR_AMD      "AuthenticAMD"
//#define VENDOR_INTEL    "GenuineIntel"

/* Linker Script Symbols */
extern void *KERNEL_LMA;
extern void *KERNEL_VMA;
extern void *KERNEL_SIZE;

/*
 * The main kernel function; this is where Ritchie begins operation.
 * The system will halt when/if this function returns.
 * Arguments:
 *   uint64_t magic: A Multiboot bootloaders magic number.
 *   uint64_t mbinfo:
 *       The physical memory address of the Multiboot information struct.
 */
void kernel_main(uint64_t magic, uint64_t *mbinfo) {
    console_clear();
    console_set_background(BG_GREY);
    console_printf(FG_BLUE, STRING"\n");

//    gdt_init();
//    idt_init();
//    idt_install_exceptions();
//    idt_install_irqs();
    //__asm__ volatile ("sti");

    /* Get Kernel size */
    uint64_t size = ((uint64_t) &KERNEL_SIZE) / 1024;
    console_printf(FG_WHITE, "Kernel size: %uKB\n", size);

    /* Get and print number of cores */
//    console_printf(FG_WHITE, "Cores: %u\n", cpuid_cores());

    /* Get CPU vendor name */
/*    char id[13] = {0};
    cpuid_vendor(id);
    console_printf(FG_WHITE, "Vendor_id: ");
*/
    /* Print CPU vendor name */
/*    if (!strncmp(id, VENDOR_INTEL, strlen(id))) {
        console_printf(FG_CYAN_L, "%s\n", id);
    }
    else if (!strncmp(id, VENDOR_AMD, strlen(id))) {
        console_printf(FG_RED_L, "%s\n", id);
    }
    else {
        console_printf(FG_GREY_L, "%s\n", id);
    }
*/
    /* Was the kernel booted by a Multiboot bootloader? */
//    if (magic == MULTIBOOT_BOOT_MAGIC) {
//        multiboot_init(mbinfo);
//        multiboot_dump();
//    }

    console_printf(FG_RED, "System halted");
    return; /* Halt the system */
}
