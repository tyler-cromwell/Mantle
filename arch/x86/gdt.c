/* Copyright (C) 2015 Tyler Cromwell <tyler@csh.rit.edu>

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
*/

/* C Standard Library Headers,
   these don't need to link against libc */
#include <stdint.h>

/* Kernel Headers */
#include <drivers/console.h>

/* A GDT segment descriptor */
struct GdtDescriptor {
    uint16_t limit;             /* Address length */
    uint16_t base_low;          /* Lower portion of the base address */
    uint8_t base_mid;           /* Middle portion of the base address */
    uint8_t access;             /* A collection of various bits */
    uint8_t granularity;        /* Another collection of various bits */
    uint8_t base_high;          /* Upper portion of the base address */
} __attribute__((__packed__));

/* The GDTR register */
struct Gdtr {
    uint16_t limit; /* The length of the GDT */
    uint32_t base;  /* GDT base address */
} __attribute__((__packed__));

/* The GDT and a pointer to it */
static struct GdtDescriptor gdt[3];
static struct Gdtr gdtr;

/* External - Load the GDT */
extern void gdt_load(struct Gdtr);

/*
 * Builds a Global Descriptor Table descriptor and writes it to the given 'index'.
 * The maximum number of entries is 8192.
 * Arguments:
 *   uint8_t index: Index in the GDT to write the entry to.
 *   uint32_t base: Descriptor base address.
 *   uint32_t limit: Descriptor limit.
 *   uint8_t access: Access byte.
 *   uint8_t gran: Granularity byte, contains the 4 'flags' and upper 4 'limit' bits.
 */
static void gdt_write_descriptor(uint8_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    struct GdtDescriptor descriptor = {
        .limit = limit & 0xffff,
        .base_low = base & 0xffff,
        .base_mid = (base >> 16) & 0xff,
        .access = access,
        .granularity = ((limit >> 16) & 0x0f) | (gran & 0xf0),
        .base_high = (base >> 24) & 0xff
    };

    gdt[index] = descriptor;
}

/*
 * Installs the Global Descriptor Table.
 */
void gdt_init(void) {
    /* Setup pointer */
    gdtr.limit = (sizeof(struct GdtDescriptor) * 4) - 1;
    gdtr.base = (uint32_t) &gdt;

    /* Initialize entries */
    gdt_write_descriptor(0, 0x0, 0x00000000, 0x00, 0x00);   /* Null, 0x00 */
    gdt_write_descriptor(1, 0x0, 0xffffffff, 0x9a, 0xcf);   /* Code, 0x08 */
    gdt_write_descriptor(2, 0x0, 0xffffffff, 0x92, 0xcf);   /* Data, 0x10 */

    gdt_load(gdtr);

    console_printf(FG_WHITE, "GDT Initialized\n");
}
