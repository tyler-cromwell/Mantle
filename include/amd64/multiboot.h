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

/*
 * Information was referenced from the GNU Multiboot Specification
 * GNU Multiboot 0.6.96 is the supported version, currently.
 */

#ifndef AMD64_MULTIBOOT_H
#define AMD64_MULTIBOOT_H

/* C Standard Library header(s) */
#include <stddef.h>

/* Kernel header(s) */
#include <kernel/types.h>

/* Macro constants */
#define MULTIBOOT_BOOT_MAGIC    0x2badb002  /* Bootloader magic number */
#define MULTIBOOT_MEMORY        1 << 0      /* Lower and Upper Memory flag */
#define MULTIBOOT_MODULES       1 << 3      /* Boot modules flag */
#define MULTIBOOT_MMAP          1 << 6      /* Memory map flag */
#define MULTIBOOT_DRIVE         1 << 7      /* Drives flag */
#define MULTIBOOT_BOOTLOADER    1 << 9      /* Bootloader Name flag */

/* Signifies that the Kernel complies with Multiboot */
struct MultibootHeader {
    dword_t magic;              /* The header identifier, must be 0x1badb002 */
    dword_t flags;              /* Feature flags, requirements (0-15), optionals (16-31).
                                   Bit 00: 4KB page alignment
                                   Bit 01: Info in 'mem_*' will be included and 'mmap_*' if supported
                                   Bit 02: Video mode info is passed
                                   Bit 16: Fields at offsets from 12-28 are valid (required unless image is ELF) */
    dword_t checksum;           /* MBZ when added to Magic and Flags */
    dword_t header_addr;        /* Multiboot header address */
    dword_t load_addr;          /* Text segment address. Must be <= header_addr */
    dword_t load_end_addr;      /* End address of Data segment.
                                   If 0, bootloader assumes Text and Data segments occupy whole image */
    dword_t bss_end_addr;       /* End address of BSS segment. If 0, bootloader assumes not BSS segment */
    dword_t entry_addr;         /* Kernel entry point address */
    dword_t mode_type;          /* 0 for linear graphics, 1 for EGA */
    dword_t width;              /* Number of columns, pixels in graphics mode, 0 in text */
    dword_t height;             /* Number of lines, pixels in graphics mode, 0 in text */
    dword_t depth;              /* Bits per pixel in graphics mode, 0 in text */
} __attribute__((__packed__));

/* Information passed to the Kernel by the Bootloader */
struct MultibootInfo {
    dword_t flags;          /* Only bits 0 to 11 are vaild, bits 04 and 05 are mutually exclusive */
    dword_t mem_lower;      /* If bit 00: Amount of lower memory */
    dword_t mem_upper;      /* If bit 00: Amount of upper memory */
    dword_t boot_device;    /* If bit 01: BIOS disk device the Kernel was loaded from (string) */
    dword_t cmdline;        /* If bit 02: Physical address of the null-terminated command line to be passed */
    dword_t mods_count;     /* If bit 03: Number of boot modules loaded */
    dword_t mods_addr;      /* If bit 03: Physical address of the first module */

    union MultibootSectionHeaders {
        /* Valid if bit 04 is set and bit 05 is not */
        struct MultibootAoutSectionHeader {
            dword_t tabsize;    /**/
            dword_t strsize;    /**/
            dword_t addr;       /**/
            dword_t reserved;   /* Must be zero */
        } aout_header __attribute__((__packed__));

        /* Valid if bit 05 is set and bit 04 is not */
        struct MultibootElfSectionHeader {
            dword_t num;    /**/
            dword_t size;   /**/
            dword_t addr;   /**/
            dword_t shndx;  /**/
        } elf_header __attribute__((__packed__));
    } section_headers;

    dword_t mmap_length;        /* If bit 06: Length of the memory map buffer (bytes) */
    dword_t mmap_addr;          /* If bit 06: Address of the memory map buffer */
    dword_t drives_length;      /* If bit 07: Total size of drive structures (bytes) */
    dword_t drives_addr;        /* If bit 07: Physical address of the first drive structure */
    dword_t config_table;       /* If bit 08: Address of the ROM configuration table */
    dword_t boot_loader_name;   /* If bit 09: Name of the Bootloader (null-terminated) */
    dword_t apm_table;          /* If bit 10: IGNORE - Made obsolete by ACPI */
    dword_t vbe_control_info;   /* If bit 11 & bit 2 in header: Physical address of VBE info from function 00h */
    dword_t vbe_mode_info;      /* If bit 11 & bit 2 in header: Physical address of VBE info from function 01h */
    word_t vbe_mode;            /* If bit 11 & bit 2 in header: Indicates video mode in the format specified in VBE 3.0 */
    word_t vbe_interface_seg;   /* If bit 11 & bit 2 in header: */
    word_t vbe_interface_off;   /* If bit 11 & bit 2 in header: */
    word_t vbe_interface_len;   /* If bit 11 & bit 2 in header: */
} __attribute__((__packed__));

/*
 * Boot Module structure.
 * May be used if multiboot_info.flags bit 3 is set
 */
struct MultibootModules {
    dword_t mod_start;  /**/
    dword_t mod_end;    /**/
    dword_t string;     /**/
    dword_t reserved;   /* Must be zero */
} __attribute__((__packed__));

/* 
 * A region of memory.
 * May be used if mutliboot_info.flags bit 6 is set
 */
struct MultibootMmap {
    dword_t size;       /* Size of this struct */
    qword_t base_addr;  /* Starting address of the region */
    qword_t length;     /* Length of the region (in bytes) */
    dword_t type;       /* Type of region (1 for available, else reserved) */
} __attribute__((__packed__));

/* May be used if mutliboot_info.flags bit 7 is set */
struct MultibootDrive {
    dword_t  size;                      /* Size this struct */
    byte_t   drive_number      : 1;     /* BIOS drive number (0 for CHS, 1 for LBA) */
    byte_t   drive_mode        : 1;     /* Access mode used by bootloader */
    byte_t   drive_cylinders   : 1;     /* Number of cylinders */
    byte_t   drive_heads       : 1;     /* Number of heads */
    byte_t   drive_sectors     : 1;     /* Number of sectors per track */
    word_t *drive_ports;                /* Null terminated array */
} __attribute__((__packed__));

/* Function prototypes */
void multiboot_init(struct MultibootInfo *mbinfo);
void multiboot_dump(void);
size_t multiboot_memsize(void);

#endif
