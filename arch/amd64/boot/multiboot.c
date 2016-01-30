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

#define MMAP_AVAILABLE  1   /* Memory available for use */

/* Multiboot information structures */
static struct MultibootInfo *info;
static struct MultibootMmap *mmap;

static struct MultibootMmap *kernel_region; /* A pointer to the available mmap region the kerne lis within */
static struct MultibootMmap kernel = {
    .size = 24,
    .base_addr = 0x00100000,    /* The address the kernel is loaded at */
    .length = 16 * 1024 * 1024, /* "Allocated" length for the kernel binary (16MB) */
    .type = 2                   /* This area is reserved */
};

/*
 * Initializes all applicable structures from information passed in from the Bootloader.
 * Argument(s):
 *   struct MultibootInfo* mbinfo: Pointer to the info struct created by the Bootloader.
 */
void multiboot_init(struct MultibootInfo *mbinfo) {
    info = mbinfo;

    /* Initialize the Memory Map */
    if (info->flags & MULTIBOOT_MMAP) {
        ulong_t ents = info->mmap_length / sizeof(struct MultibootMmap);
        mmap = (struct MultibootMmap*) (ulong_t) info->mmap_addr;

        for (ulong_t i = 0; i < ents; i++) {
            /* If the kernel region fits in the first available region */
            if (mmap[i].type == MMAP_AVAILABLE &&
                mmap[i].length >= kernel.length) {

                /* Bookmark for later */
                kernel_region = mmap + (i * sizeof(struct MultibootMmap));
                break;
            }
        }
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
            info->mem_upper /= CONVERT_NUM;
            console_printf(FG_WHITE, "Upper Memory: %uMB\n", info->mem_upper);
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
            if (mmap[i].type == MMAP_AVAILABLE) {
                console_printf(FG_WHITE, "Available");
            } else {
                console_printf(FG_GREY, "Reserved");
            }

            console_printf(FG_WHITE, ")\n");
        }
    }
}

/*
 * Returns the total amount of system memory (the sum
 * of lower and upper memory), measured in KILOBYTES.
 * Returns:
 *   the total memory or 0 if not passed from bootloader.
 */
size_t multiboot_memsize(void) {
    if (info->flags & MULTIBOOT_MEMORY) {
        return info->mem_lower + info->mem_upper;
    } else {
        return 0;
    }
}
