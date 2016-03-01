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

/* C Standard Library header(s) */
#include <stddef.h>

/* Kernel header(s) */
#include <amd64/console.h>
#include <amd64/multiboot.h>
#include <kernel/types.h>
#include <lib/string.h>

/* Macros for converting byte metrics */
#define CONVERT_NUM 1024
#define CONVERT_UP(number) (number) / CONVERT_NUM
#define CONVERT_DOWN(number) (number) * CONVERT_NUM

/* Memory Map entry types */
#define MMAP_AVAILABLE          1   /* Memory available for use */
#define MMAP_RESERVED           2   /* Unusable memory */
#define MMAP_ACPI_RECLAIMABLE   3   /* Ususable once finished with ACPI tables */
#define MMAP_NVSRAM             4   /* Non-volatile static RAM */
#define MMAP_BAD_RAM            5   /* Faulty RAM */

/* Multiboot information structures */
static struct MultibootInfo *info;
static struct MultibootMmap *mmap;

/*
 * Saves pointers to Multiboot structures.
 * Argument(s):
 *   struct MultibootInfo* mbinfo: Pointer to the info struct.
 */
void multiboot_init(struct MultibootInfo *mbinfo) {
    info = mbinfo;

    if (info->flags & MULTIBOOT_MMAP) {
        mmap = (struct MultibootMmap*) (ulong_t) info->mmap_addr;
    }
}

/*
 * Dumps Multiboot information to the console
 */
void multiboot_dump(void) {
    /* Dump the Bootloader name */
    if (info->flags & MULTIBOOT_BOOTLOADER) {
        console_printf(FG_WHITE, "Bootloader: ");
        console_printf(FG_BROWN, "%s\n", info->boot_loader_name);
    }

    /* Dump number of boot modules */
    if (info->flags & MULTIBOOT_MODULES) {
        console_printf(FG_WHITE, "Number of boot modules: %u\n", info->mods_count);
    }

    /* Dump the amount of Lower and Upper Memory */
    if (info->flags & MULTIBOOT_MEMORY) {
        console_printf(FG_WHITE, "Lower Memory: %uKB\n", info->mem_lower);

        if (info->mem_upper >= CONVERT_NUM) {
            dword_t mem = info->mem_upper / CONVERT_NUM;
            console_printf(FG_WHITE, "Upper Memory: %uMB\n", mem);
        } else {
            console_printf(FG_WHITE, "Upper Memory: %uKB\n", info->mem_upper);
        }
    }

    /* Dump the Memory Map */
    if (info->flags & MULTIBOOT_MMAP) {
        console_printf(FG_WHITE, "\nMemory Map:\n");

        ulong_t ents = info->mmap_length / sizeof(struct MultibootMmap);

        for (ulong_t i = 0; i < ents; i++) {
            struct ItoaOptions opts = {0};
            memset(&opts, 0, sizeof(struct ItoaOptions));
            opts.pad = 1;
            opts.hex = 1;

            /* Region Base Address */
            ulong_t n = mmap[i].base_addr;
            char addr[17] = {0};

            memcpy(addr, itoa(&opts, n), 17);
            if (ents >= 10 && i < 10) {
                console_printf(FG_WHITE, "[ %u]: 0x%s - ", i, addr);
            } else {
                console_printf(FG_WHITE, "[%u]: 0x%s - ", i, addr);
            }

            /* Region Ending Address */
            n = mmap[i].base_addr + mmap[i].length - 1;
            memcpy(addr, itoa(&opts, n), 17);
            console_printf(FG_WHITE, "0x%s (", addr);

            /* Region length */
            n = CONVERT_UP(mmap[i].length);
            if (n >= CONVERT_NUM) {
                n = CONVERT_UP(n);
                console_printf(FG_WHITE, "%uMB, ", n);
            } else {
                console_printf(FG_WHITE, "%uKB, ", n);
            }

            /* Region type */
            switch (mmap[i].type) {
                case MMAP_AVAILABLE:
                    console_printf(FG_WHITE, "Available");
                    break;
                case MMAP_RESERVED:
                    console_printf(FG_GREY, "Reserved");
                    break;
                case MMAP_ACPI_RECLAIMABLE:
                    console_printf(FG_BROWN_L, "ACPI Reclaimable");
                    break;
                case MMAP_NVSRAM:
                    console_printf(FG_GREY, "NVSRAM");
                    break;
                case MMAP_BAD_RAM:
                    console_printf(FG_RED, "Bad RAM");
                    break;
            }

            console_printf(FG_WHITE, ")\n");
        }
    }
}

/*
 * Returns the total amount of system memory, measured in KILOBYTES.
 * Returns:
 *   the total memory or 0 if not passed from bootloader.
 */
size_t multiboot_memsize(void) {
    if (info->flags & MULTIBOOT_MMAP) {
        return info->mem_lower + info->mem_upper;
    } else {
        return 0;
    }
}
